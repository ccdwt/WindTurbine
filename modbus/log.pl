#!/usr/bin/perl 
use strict;
use IO::Socket;
use XML::Simple;
use threads;

use constant MAX_RECV_LEN => 65536;

#### Code based off code from Andrzej Sokulski WWW.MODBUS.PL ###

##################### reading configuration from xml file #################################

my $file = '/usr/src/WindTurbine/bin/config.xml';             #Configuration file
my $xs1 = XML::Simple->new();
my $doc = $xs1->XMLin($file)
or die "Modbus client : problem with congig file: $file\n";;
my $simple_tcp_port = $doc->{SERVER}->{PEERPORT};
my $LOGDIR = $doc->{SERVER}->{LOGDIR};
my $PIDDIR = $doc->{SERVER}->{PIDDIR};

my $DEBUG = "false";

my $calledName = $0;
print "called as $calledName\n" if $DEBUG;
my $remote_host; 
my $logfile;
my $pidfile;
if ($calledName =~ m/rowland/){
	$remote_host = "rowland_pwr";
	$logfile = "$LOGDIR/power_rowland.csv";
	$pidfile = "$PIDDIR/power_rowland.pid";
} elsif ($calledName =~ m/turbine/){
	$remote_host = "turbine_pwr";
	$logfile = "$LOGDIR/power_turbine.csv";
	$pidfile = "$PIDDIR/power_turbine.pid";
}
my $remote = shift || $remote_host;
my $remote_port = $simple_tcp_port;
my $log = shift  || $logfile;
my $lock = shift || $pidfile;
my $polls_time = $doc->{REGISTERS}->{POLLSTIME};
my $trans_serv = getprotobyname( 'tcp' );
$remote_host = gethostbyname( $remote )
or die "Modbus client : name lookup failed: $remote\n";
my $destination = sockaddr_in( $remote_port, $remote_host );
my $time = time() - $polls_time;

#################### end of configurations #############################################

#################### Open Log file #####################################################

open (LOCK, ">$lock") or die "cannot open lockfile: $lock";
	print LOCK "$$\n";
close(LOCK);

open (LOG_FILE, ">$log") or die "cant open $log for logging";
LOG_FILE->autoflush(1);

#print LOG_FILE "starting log on " . localtime . "\n";

#################### end of Log File ###################################################

#################### Start Loop ########################################################

while(1){
########### check lockfile ########################

	open (LOCK, "<$lock") or die "cannot open lockfile: $lock";
	my $lockPid = <LOCK>;
	chomp $lockPid;
	if ($lockPid ne $$){
		# we are no longer the holder of the lockfile
		exit(1);
	}
	close(LOCK);

################ end check lockfile ################
	my @line;
	my $line;
	sleep ($polls_time - (time() - $time)) unless (time()-$time > $polls_time); 
	foreach my $group (@{$doc->{REGISTERS}->{GROUP}}){
		$time = time();
		push @line, @{GetGroup($group)} ;
	}
	if (scalar(@line) == 16){
		$line =sprintf time . "," .join(',', @line) ;
		printf LOG_FILE $line . "\n";
	}
##	print  join(', ', @line) . "\n";
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
			my $signed = undef; #unsigned

			if ($group->{SIGNED} eq "TRUE"){ 
				#if (unpack("C",substr($data,$start,1)) != 0){
					$signed = "TRUE"; # signed
			}

			for (0 .. $len-1){
				my $v = unpack("n",substr( $data,($start + $_) *2+9,2));
				if ($signed && $v > (65536/2)){
					$v+= -65536;
				}
				$value += ($len-$_ ** (65536)) * $v;
			}	
	

			$value *= $group->{MULTIPLIER};
			push @return, "$value";
#			print "$value $units";
		}

	} else {
		warn "Modbus client : problem with recv: $!\n";
	}
	

	close TCP_SOCK
		or warn "tcp_c1: close failed: $!\n";   
	return \@return;
}
