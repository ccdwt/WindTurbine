#!/usr/bin/perl 
use strict;
use IO::Socket;
use XML::Simple;
use threads;

use constant MAX_RECV_LEN => 65536;

#### Code based off code from Andrzej Sokulski WWW.MODBUS.PL ###

##################### reading configuration from xml file #################################

my $file = 'config.xml';             #Configuration file
my $xs1 = XML::Simple->new();
my $doc = $xs1->XMLin($file)
or die "Modbus client : problem with congig file: $file\n";;
my $simple_tcp_port = $doc->{SERVER}->{PEERPORT};
my $remote_host = $doc->{SERVER}->{PEERADDR};
my $logFile = $doc->{LOGFILE};
my $remote = shift || $remote_host;
my $remote_port = shift || $simple_tcp_port;
my $log = shift || $logFile;
my $polls_time = $doc->{REGISTERS}->{POLLSTIME};
my $trans_serv = getprotobyname( 'tcp' );
$remote_host = gethostbyname( $remote )
or die "Modbus client : name lookup failed: $remote\n";
my $destination = sockaddr_in( $remote_port, $remote_host );
my $time = time() - $polls_time;

#################### end of configurations #############################################

#################### Open Log file #####################################################

open (LOG_FILE, ">$log") or die "cant open $log for logging";
LOG_FILE->autoflush(1);

print LOG_FILE "starting log on " . localtime . "\n";

#################### end of Log File ###################################################

#################### Start Loop ########################################################

while(1){
	my @line;
	sleep ($polls_time - (time() - $time)) unless (time()-$time > $polls_time); 
	foreach my $group (@{$doc->{REGISTERS}->{GROUP}}){
		$time = time();
		push @line, @{GetGroup($group)} ;
	}
	print LOG_FILE time . ", " .join(', ', @line) . "\n";
	print  join(', ', @line) . "\n";
}

close(LOG);

sub GetGroup{
	my $group = shift;

	my @return;
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

	socket( TCP_SOCK, PF_INET, SOCK_STREAM, $trans_serv )
	      or die "Modbus client: socket creation failed: $!\n";
	my $con_ok = connect( TCP_SOCK, $destination )
	      or warn "Modbus client: connect to remote system failed: $!\n";
	      next unless $con_ok;

################### server connected ###################################################

################### reading registers ################################################## 
	my $data = join('',@buffor);                           #Modbus frame to scalar
	send( TCP_SOCK, $data, 0 )
	or warn "Modbus client : problem with send: $!\n";
	my $from_who = recv( TCP_SOCK, $data, MAX_RECV_LEN, 0 );
	if ( $from_who || $data ){                                     #New data from server   
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

			print unpack("n",substr($data,9,2)) . "\n" if $value >100;
			if (($group->{SIGNED} eq "TRUE") && (unpack("n",substr($data,0,9)) != 0)) {
				$value -= (65536) ** ($len);
			}

			$value *= $group->{MULTIPLIER};
			push @return, "$value $units";
#			print "$value $units";
		}

	} else {
		warn "Modbus client : problem with recv: $!\n";
	}
	

	close TCP_SOCK
		or warn "tcp_c1: close failed: $!\n";   
	return \@return;
}
