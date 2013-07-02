#!/usr/bin/perl
use strict;
use IO::Socket;
use XML::Simple;
use Data::Dumper;
use threads;

use constant MAX_RECV_LEN => 65536;

#### Andrzej Sokulski WWW.MODBUS.PL ###

##################### reading configuration from xml file #################################

my $file = 'config.xml';             #Configuration file
my $xs1 = XML::Simple->new();
my $doc = $xs1->XMLin($file)
or die "Modbus client : problem with congig file: $file\n";;
my $simple_tcp_port = $doc->{SERVER}->{PEERPORT};
my $remote_host = $doc->{SERVER}->{PEERADDR};
my $remote = shift || $remote_host;
my $remote_port = shift || $simple_tcp_port;
#my $quantity = $doc->{REGISTERS}->{QUANTITY};
#my $polls_time = $doc->{REGISTERS}->{POLLSTIME};
#my $nr_of_polls = $doc->{REGISTERS}->{NROFPOLLS};
my $trans_serv = getprotobyname( 'tcp' );
$remote_host = gethostbyname( $remote )
or die "Modbus client : name lookup failed: $remote\n";
my $destination = sockaddr_in( $remote_port, $remote_host );
my @threads = ();

#################### end of configurations #############################################

#################### creating the modbus frame #########################################
foreach my $group (@{$doc->{REGISTERS}->{GROUP}}){
	push @threads, threads->create(\&GetGroup,$group);
}

print "spun off threads.\n";

for (@threads){
	$_->join();
}

sub GetGroup{
	my $group = shift;
	my $socket; #= TCP_SOCK;

	my $quantity = $group->{LENGTH};
	my $msg_count = 0;
	my $start_add = $group->{STARTADDR} - 40001;
	my $pack_val = unpack("H*",pack("n",$start_add));
	my $units = $group->{UNITS};
	my @buffor = ();
	$buffor[0 ] = chr(0 ); # 1
	$buffor[1 ] = chr(0 );
	$buffor[2 ] = chr(0 );
	$buffor[3 ] = chr(0 );
	$buffor[4 ] = chr(0 );
	$buffor[5 ] = chr(6 );
	$buffor[6 ] = chr(1 );
	$buffor[7 ] = chr(3 );
	$buffor[8 ] = chr(hex (substr $pack_val, 0, 2));
	$buffor[9 ] = chr(hex (substr $pack_val, 2, 2));
	$buffor[10] = chr(0 );
	$buffor[11] = chr($quantity);

#################### end of modbus frame ###############################################

#################### connecting to the server ##########################################

	socket( $socket, PF_INET, SOCK_STREAM, $trans_serv )
	      or die "Modbus client: socket creation failed: $!\n";
	my $con_ok = connect( $socket, $destination )
	      or warn "Modbus client: connect to remote system failed: $!\n";
	      next unless $con_ok;

################### server connected ###################################################

################### reading registers ################################################## 
	my $data = join('',@buffor);                           #Modbus frame to scalar
	send( $socket, $data, 0 )
	or warn "Modbus client : problem with send: $!\n";
	my $from_who = recv( $socket, $data, MAX_RECV_LEN, 0 );
	if ( $from_who ){                                     #New data from server   
		my ( $the_port, $the_ip ) = sockaddr_in( $destination );
		my $remote_name = gethostbyaddr( $the_ip, AF_INET );
		my $values;

		if (ref($group->{VALUE}) eq 'ARRAY'){
			$values = $group->{VALUE};
		} else {
			$values = [$group->{VALUE},];
		}
			
		foreach(@{$values}){
			my $start = $_->{OFFSET};
			my $len = $_->{LENGTH};
			my $name = $_->{NAME};
			my $value;

			for (0 .. $len-1){
				$value += ($len-$_ ** (65536)) * 
					unpack("n",substr( $data, ($start +$_)*2+9, 2));
			}
			print "Value = $value\n";

			if (($group->{SIGNED} eq "TRUE") && (unpack("n",substr($data,10,1)) != 0)) {
				$value -= (65536) ** ($len);
			}

			$value *= $group->{MULTIPLIER};
			print "$name\t= $value $units\n";
		}

	} else {
		warn "Modbus client : problem with recv: $!\n";
	}

close($socket)
or warn "tcp_c1: close failed: $!\n";   
}
