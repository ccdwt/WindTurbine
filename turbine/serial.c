#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "serial.h"

int serial_wait_for_read(unsigned int Seconds, unsigned int USeconds);
static struct termios 	pots;
int			FileDesc;
struct serial_port_options serial_port = {NULL,B9600,8,1,0,-1};


void print_InHex(char* arry, int len){
        int i = 0;
        for (i = 0; i < len; i++){
                printf("%3.2x" , (unsigned char)arry[i]);
        }
}

/* restore original terminal settings on exit */

void cleanup_termios(int signal)
{
    tcsetattr(FileDesc, TCSANOW, &pots);
    exit(0);
}

void IO_write(char *cmd, size_t length){
//	print_InHex(cmd,length);
//	printf("\n\n");
	write(FileDesc,cmd,length);
}

ssize_t IO_read(char * buf, size_t count, unsigned int uSec){
	if (serial_wait_for_read(uSec/1000000,uSec%1000000) > 0){
		fcntl(FileDesc,F_SETFL,0| O_NONBLOCK);// nonblocking io;
		return read(FileDesc,(void*) buf, count);
		fcntl(FileDesc,F_SETFL,0);
	}
	return(0);
}

int serial_wait_for_read(unsigned int Seconds, unsigned int USeconds){
	fd_set set;
	struct timeval timeout;
	int rv;

	FD_ZERO(&set); // clear our fild_descriptor set;
	FD_SET(FileDesc, &set); // add the serial port to our filedescriptor set.
	
	timeout.tv_sec = Seconds;
	timeout.tv_usec = USeconds;
	
	return select(FileDesc+1, &set, NULL,NULL,&timeout);
}
	
int IO_init(){
    struct  termios pts;  /* termios settings on port */
    struct  sigaction sact;/* used to initialize the signal handler */
    char key;

/* Section to configure COM port */

    if (serial_port.name == NULL){
	perror("Cannot open NULL port: you must set the serial port name");
	exit(2);
    }

    FileDesc = open( serial_port.name, O_RDWR);
    if (FileDesc < 0) {
       perror("problem opening COM port");
       exit(2);
    }

    serial_port.fd = FileDesc;

    /* modify the port configuration */
    tcgetattr(FileDesc, &pts);
    pots = pts;
    pts.c_lflag &= ~ICANON; 			// make sure no canonical mode
    pts.c_lflag &= ~(ECHO | ECHOCTL | ECHONL);  // remove echo if it was there
    //pts.c_cflag |= HUPCL;			// hang up
    switch (serial_port.parity){	// set parity
	case 0: //none
		pts.c_cflag &= ~PARENB;	// disable parity;
    		pts.c_cflag &= ~PARODD;	// remove ODD parity
		break;
	case 1: // odd
		pts.c_cflag |= PARENB; 	// enable parity;
		pts.c_cflag |= PARODD; 	// odd parity
		break;
	case 2: // even
		pts.c_cflag |= PARENB;  // enable parity;
		pts.c_cflag &= ~PARODD; // make sure odd parity is not set.
		break;

	default:
		perror("cannot use parity values other than 0:NONE 1:ODD or 2:EVEN");
    }
    pts.c_cflag &= ~(CSIZE); 		// clear size mask;
    switch (serial_port.data_bits){	// set data bits
	case 5: 			// 5 data bits
		pts.c_cflag |= CS5;
		break;
	case 6: 			// 6 data bits
		pts.c_cflag |= CS6;
		break;
	case 7: 			// 7 data bits
		pts.c_cflag |= CS7;
		break;
	case 8: 			// 8 data bits
		pts.c_cflag |= CS8;
		break;
	default: 			// error: invalid input
		perror("serial_port.data_bits must be 5,6,7 or 8");
	}
    switch (serial_port.stop_bits){ 	// set stop bits.
	case 1: 
		pts.c_cflag &= ~CSTOPB;	// make sure there are not two stop bits;
		break;
	case 2:
		pts.c_cflag |= CSTOPB;	// make sure there are two stop bits;
		break;
	default: 			//error
		perror("Invalid number of stop bits. must be either 1 or 2");
	}
    pts.c_cc[VMIN] = 1; 		// minimum read is 1 character
    pts.c_cc[VTIME] = 0;
    pts.c_cc[VINTR] = 0;		// disable interrupts;
    pts.c_cc[VQUIT] = 0;
    pts.c_oflag &= ~ONLCR; 		// dont convert \n -> \r\n
    pts.c_iflag &= ~ICRNL; 		// same but for input
    pts.c_cflag &= ~CRTSCTS;		// disable hardware flow control;
    pts.c_iflag &= ~(IXON|IXOFF|IXANY); // disable XON/XOFF flow controll;

    cfsetospeed(&pts, serial_port.baud);   
/* All this sets the port to UPS defaults */

    /* set the signal handler to restore the old
 *      termios handler */
    sact.sa_handler = cleanup_termios;
    sigaction(SIGHUP, &sact, NULL);
    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGPIPE, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);
    tcsetattr(FileDesc, TCSANOW, &pts);
}

void IO_close(){
	tcsetattr(FileDesc, TCSANOW, &pots);
	close(FileDesc);
}
