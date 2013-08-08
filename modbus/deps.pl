use strict;
use IO::Socket;
use XML::Simple;
use warnings;
use Proc::Daemon;
use constant MAX_RECV_LEN => 65536;
