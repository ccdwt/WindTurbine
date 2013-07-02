#!/usr/bin/perl
use strict;
use IO::Socket;
use XML::Simple;

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
my $start_add = $doc->{REGISTERS}->{STARTADDR} - 1;
my $quantity = $doc->{REGISTERS}->{QUANTITY};
my $polls_time = $doc->{REGISTERS}->{POLLSTIME};
my $nr_of_polls = $doc->{REGISTERS}->{NROFPOLLS};
my $data_format = $doc->{REGISTERS}->{DATAFORMAT};
my $trans_serv = getprotobyname( 'tcp' );
$remote_host = gethostbyname( $remote )
or die "Modbus client : name lookup failed: $remote\n";
my $destination = sockaddr_in( $remote_port, $remote_host );
my $pack_val = unpack("H*",pack("n",$start_add));
my $msg_count = 0;

#################### end of configurations #############################################

#################### creating the modbus frame #########################################

my @buffor = ();
$buffor[0 ] = chr(0 ); # 1
$buffor[1 ] = chr(0 );
$buffor[2 ] = chr(0 );
$buffor[3 ] = chr(0 );
$buffor[4 ] = chr(0 );
$buffor[5 ] = chr(6 );
$buffor[6 ] = chr(1 );
$buffor[7 ] = chr(3 );
$buffor[8 ] = chr(0 ); #(hex (substr $pack_val, 0, 2));
$buffor[9 ] = chr(10); #chr(hex (substr $pack_val, 2, 2));
$buffor[10] = chr(0 );
$buffor[11] = chr(3 ); #chr($quantity);

#################### end of modbus frame ###############################################

#################### connecting to the server ##########################################

socket( TCP_SOCK, PF_INET, SOCK_STREAM, $trans_serv )
      or die "Modbus client: socket creation failed: $!\n";
my $con_ok = connect( TCP_SOCK, $destination )
      or warn "Modbus client: connect to remote system failed: $!\n";
      next unless $con_ok;
################### server connected ###################################################

################### reading registers ################################################## 
while ( $msg_count < $nr_of_polls){
       my $data = join('',@buffor);                           #Modbus frame to scalar
       send( TCP_SOCK, $data, 0 )
              or warn "Modbus client : problem with send: $!\n";
        my $from_who = recv( TCP_SOCK, $data, MAX_RECV_LEN, 0 );
        if ( $from_who ){                                     #New data from server   
            my ( $the_port, $the_ip ) = sockaddr_in( $destination );
            my $remote_name = gethostbyaddr( $the_ip, AF_INET );
            my $i;
            my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time); 
            print "\n $hour:$min:$sec  >>  New data from Modbus TCP/IP $remote_name server \n";
            for ($i = 0; $i < (length( $data ) - 9)/2;$i++){
                my $hex = substr $data, $i*2+9, 2;
                if ($data_format eq "HEX"){
                  $hex = unpack "H*", $hex;
                  } 
                else {
                  $hex = unpack "n", $hex;
                  }
                my  $regadd = 400001 + $start_add  + $i;
                print " $regadd : [$hex] \n";
            }

}
else
{
warn "Modbus client : problem with recv: $!\n";

}

}
continue {
$msg_count++;
sleep($polls_time);
}

close TCP_SOCK
or warn "tcp_c1: close failed: $!\n";   
