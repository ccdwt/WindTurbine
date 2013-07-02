#ifndef __MY_USB_SERIAL_H_
#define __MY_USB_SERIAL_H_

int myusb_init();
int myusb_write( char * mem, size_t len);
int myusb_read( char * mem, size_t len, int timeout);
void myusb_close();

#endif
