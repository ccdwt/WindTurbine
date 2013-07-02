/* IO.h this file is used to include the actual io library. It also defines
 * a few helper functions.
 * Written by Paul Martin for Calvin College 06/03/13 */
#ifndef _IO_H__
#define _IO_H__

#include <stdio.h>
#include "serial.h"
//#include "myusb.h"
//#include "usbserial.h"

void printInHex(char* arry, int len);
void printOut(char* arry, int len);


#endif
