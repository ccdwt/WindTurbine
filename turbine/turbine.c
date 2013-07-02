#include <stddef.h>
#include "SIP.h"
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
		XB_AT(0,"ND",NULL,0);
		for (cnt = 0; cnt < 64; cnt ++){ 

			int len = SIP_Request(cnt,SIP);
			//printf ("transmitting ..."); fflush(stdout);
			XB_TX(cnt, SIP,len);
			//printf ("sent. parsing..."); fflush(stdout);
			if ((RX_data.len != 0) && (SIP_Parse(RX_data.data, RX_data.len) )) {
				printf("Power output = %3d W\t", Turbine_Status.Pout);
				printf("RPM = %3d RPM\t", Turbine_Status.AltRPM);
				printf("Total generated = %7d Wh\t", Turbine_Status.AccWh);
				if(Turbine_Status.SysStatus & ST_RUN) 
					printf("\x1B[32mRUN\x1B[0m");
				if(Turbine_Status.SysStatus & ST_WAITING)
					printf("\x1B[33mWAITING\x1B[0m");

				printf("\r");
				fflush(stdout);
			}
			//printf("parsed!\n");
			RX_data.len = 0;
		}
	}
	IO_close();
}	
