/* Basic driver for serial communications */

#ifndef __WIND_SERIAL_H__
#define __WIND_SERIAL_H__

#include <termios.h>

struct serial_port_options {
	char* name;
	speed_t baud;
	unsigned int data_bits;
	unsigned int stop_bits;
	unsigned int parity;
	int fd;
};

extern struct serial_port_options serial_port;

/* defaults */
// serial_port.name = NULL;  /* no default, will fail without being changed*/
// serial_port.speed = B9600;
// serial_port.data_bits = 8;
// serial_port.parity = 0; // 0= none 1= odd 2 = even;
// serial_port.stop_bits = 1; 
// serial_port.fd = -1; // start with invalid port;

int IO_init();
void IO_write(char* buf, size_t length);
//ssize_t IO_read(char *buf, size_t count);
ssize_t IO_read(char *buf, size_t count, unsigned int uSec);
//int IO_wait_for_read(unsigned int Seconds, unsigned int U_Seconds);
void IO_close();

#endif // end of header
