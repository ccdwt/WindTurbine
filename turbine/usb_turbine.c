#include <stdio.h>
#include "SIP.h"
#include "myusb.h"
//#include "serial.h"

/* utility function for visualization
 * takes in an array pointer and a length.
 * outputs in hex
 */
void printInHex(char* arry, int len){
	int i = 0;
	for (i = 0; i < len; i++){
		printf("%3.2x" , (unsigned char)arry[i]);
	}
}

unsigned char XBEE_checksum(unsigned char* mem, size_t len){
	unsigned char total = 0;
	int i;
	for (i = 0; i < len; i++){
		total += mem[i];
	}
	return 0xff - total;
}

void XB_AT_NJ(int cnt){
	char packet[9];
	char BUF[100];

	packet[0] = 0x7e;
	packet[1] = 0x00;
	packet[2] = 0x04;
	packet[3] = 0x08;
	packet[4] = 0x01 + cnt;
	packet[5] = 0x4e;
	packet[6] = 0x4a;
	packet[7] = 0x00;
	packet[8] = XBEE_checksum(&packet[3], 4);
	printInHex(packet,9);
	printf("\nwrote %d bytes\n", myusb_write(packet, 9));
	int len = 0;
	len += myusb_read(BUF,100,100);
	printf("read %d bytes\n",len);
	if (len > 0){
		printf ("Got Response:\n");
		printInHex(BUF,len);
		printf("\n");
		sleep (1);
	}
}

void XB_AT_ND(int cnt){
	char packet[8];
	char BUF[100];

	packet[0] = 0x7e;
	packet[1] = 0x00;
	packet[2] = 0x04;
	packet[3] = 0x08;
	packet[4] = 0x01 + cnt;
	packet[5] = 0x4e;
	packet[6] = 0x44;
	packet[7] = XBEE_checksum(&packet[3], 4);
	printInHex(packet,8);
	printf("\nwrote %d bytes\n", myusb_write(packet, 8));
	int len = 0;
	len += myusb_read(BUF,100,100);
	printf("read %d bytes\n",len);
	if (len > 0){
		printf ("Got Response:\n");
		printInHex(BUF,len);
		printf("\n");
		sleep (1);
	}
}

int main(int argc, char *argv[]) {
	
	char Prefix[17 + 15 + 1];
	char * SIP = &Prefix[17]; 
	int cnt;
	int input;
	unsigned long len;
	char BUF[100];
	//serial_port.name = "/dev/ttyS3";
	//serial_init();
	if (myusb_init() != 0){
		return(-7);
	}
	for (cnt = 0; cnt < 64; cnt ++){ 

 		Prefix[ 0] = 0x7e;	// Start of header
		Prefix[ 1] = 0x00;	// |¯
		Prefix[ 2] = 0x1d;	// |_ Length
		Prefix[ 3] = 0x10;	// 
		Prefix[ 4] = 0x01+cnt;  // 
		Prefix[ 5] = 0x00;	//
		Prefix[ 6] = 0x13;	//
		Prefix[ 7] = 0xa2;	//
		Prefix[ 8] = 0x00;	//
		Prefix[ 9] = 0x40;	//
		Prefix[10] = 0x32;	//
		Prefix[11] = 0xcd;	//
		Prefix[12] = 0x83;	//
		Prefix[13] = 0xff;	// 90
		Prefix[14] = 0xfe;	// 97
		Prefix[15] = 0x00;	//
		Prefix[16] = 0x00; 	//
		

		SIP[ 0] = 0x10; 	//    DLE
		SIP[ 1] = 0x01;		//    SOH
		SIP[ 2] = 0x00;  	// |¯
		SIP[ 3] = 0x00;  	// | 
		SIP[ 4] = 0x00;  	// |  Address
		SIP[ 5] = 0x0A;  	// |_
		SIP[ 6] = cnt;   	//    SYNC
		SIP[ 7] = 0x00;  	// |¯
		SIP[ 8] = 0x05;  	// |_ Count
		SIP[ 9] = 0xd5 + 2*cnt;	// |¯        
		SIP[10] = 0x8a - 5*cnt;	// |_ Security
		unsigned int crc = calc_crc16(&SIP[2], 9);
		SIP[11] = crc%256;	// |¯   
		SIP[12] = crc>> 8;	// |_ CRC
		SIP[13] = 0x10;   	//    DLE
		SIP[14] = 0x03;   	//    ETX 
		Prefix[32] = XBEE_checksum(&Prefix[3], 29);
		printInHex(Prefix,33);
		//printInHex(SIP,15);
		printf("\n");
		printf("wrote %d bytes\n", myusb_write(Prefix, 33));
		sleep(.1); // give the buffer some time to fill up
		len = 0;
		len += myusb_read(BUF,100,100);
		printf("read %d bytes\n",len);
		if (len > 0){
			printf ("Got Response:\n");
			printInHex(BUF,len);
			printf("\n");
			sleep (1);
		}
		
	}
	myusb_close();
}	
