#include <stddef.h>
#include "SIP.h"
#define __DEBUG
#include "XB.h"
#include "io.h"



int main(int argc, char *argv[]) {
	
	char SIP[15];
	int cnt;
	int input;
	ssize_t len;
	char BUF[256];
#ifdef __WIND_SERIAL_H__
	serial_port.name = "/dev/ttyO1";
	serial_port.baud = B115200;
	serial_port.data_bits = 8;
#endif
	IO_init();
	while (1){
		for (cnt = 0; cnt < 64; cnt +=2){ 
			int len = SIP_Request(cnt+1,SIP);
			XB_AT(cnt,"ND",NULL,0);
			XB_TX(cnt+1,SIP,len);

			if ((RX_data.len != 0)) {
			}
			RX_data.len = 0;
		}
	}
	IO_close();
}	
