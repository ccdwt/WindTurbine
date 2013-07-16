#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "XB.h"
#include "io.h"

#define __DEBUG

unsigned char addr64[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff};//{0x00,0x13,0xa2,0x00,0x40,0x32,0xcd,0x83};
unsigned char addr16[2] = {0xff,0xfe}; //{0xf9,0x85};
buffer RX_data = (buffer){ .len = 0};


void parse(char* packet, size_t len){
	packet_t latest;
#ifdef __DEBUG
	printf("\x1B[32mPACKET VALID\n");
	printf("\x1B[33m> ");printInHex(packet,len);printf("\n");
#endif
	latest.len = packet[1]* 256 + packet[2];
	latest.type = packet[3];
	switch (latest.type){
		case 0x85:
			latest.addr16[0] = packet[4];
			latest.addr16[1] = packet[5];
			memcpy(latest.addr64,&packet[6],8);
			memcpy(latest.data,&packet[14],latest.len - 11);
#ifdef __DEBUG
			printf("ND found %s",latest.data);
#endif
			break; 
		case 0x88:
			latest.seq = packet[4];
			latest.cmd[0] = packet[5];
			latest.cmd[1] = packet[6];
			latest.opts = packet[7];
			memcpy(latest.data,&packet[8], latest.len - 5);
			if (latest.cmd[0] == 'N' && latest.cmd[1] =='D'){
				
			}
			break;
		case 0x90:
			memcpy(addr64,&packet[4],8);
			memcpy(addr16,&packet[12],2);
			latest.opts = packet[14];
			memcpy(latest.data,&packet[15],latest.len - 12);
			memcpy(&RX_data.data[RX_data.len], &packet[15], latest.len -12);
			RX_data.len += latest.len-12;
			break;
	}


}

void valid_parse(unsigned char* data, size_t len){
	static buffer packet;
	memcpy ( &packet.data[packet.len], data, len);

#ifdef __DEBUG
	printf("checking data: "); printInHex(packet.data,packet.len); printf("\n");
#endif
	packet.len += len;
#ifdef __DEBUG
	printf("packet.len = %d\n",packet.len);
#endif
	int i;
	unsigned char total = 0;
	if (packet.data[0] == 0x7e){
		int packet_len = 256*packet.data[1] + (packet.data[2]) + 4;
#ifdef __DEBUG
		printf(" packet should be length %d\n",packet_len);
#endif
		
		if (packet.len == packet_len){
			for (i = 3; i < packet.len-1; i++){
				total += packet.data[i];
			}
			if (0xff - total == packet.data[packet.len-1]){ // valid packet
				len = packet.len;
				packet.len = 0;
#ifdef __DEBUG
				printf("VALID PACKET !!!!\n");
#endif
				parse(packet.data,len);
				return;
			}
#ifdef __DEBUG	
			printf("\x1B[31mINVALID CHECKSUM %3.2x%15s\x1B[0m\n"," ",0xff-total);
#endif
			return;
		} else if (packet_len < packet.len){
			parse(packet.data, packet_len);
			len = packet.len;
			packet.len = 0;
			valid_parse(&packet.data[packet_len], len - packet_len);
			return;
		}
#ifdef __DEBUG	
		printf("\x1B[31mWRONG LENGTH%15s\x1B[0m\n"," ");
#endif
		return;
	}
       	while ( packet.data[0] != 0x7e && packet.len > 0){
#ifdef __DEBUG	
		printf("finding a 7e length = %d\n",packet.len);
#endif		
		packet.len --;
		memmove(packet.data, &packet.data[1], packet.len);
	}
#ifdef __DEBUG	
	printf("\x1B[31mWRONG MAGIC NUMBER\x1B[33m\n");
#endif
	packet.len = 0;
	return;
}


void listen(unsigned int mSec){
        char BUF[256];
        char INPUT[2048];
        unsigned int input=0;
#ifdef __DEBUG	
	printf("\x1B[33m"); // text will be yellow
#endif
        int len = IO_read(BUF,256,mSec*1000); // wait to see if there is return
        while (len > 0){
		//printf("%d bytes in buffer: ",len); printInHex(BUF,len); printf("\n");
//                if ((BUF[0] == 0x7e) && (input != 0)){ // deal with multiple packets
#ifdef __DEBUG
//                      printf ("\x1B[33m> ");
//               	printInHex(BUF,len);
//                      printf("\ninput = %d\n", len);
#endif
//			valid_parse(BUF,len);
//			input = 0;
//                }
		memcpy(&INPUT[input], BUF,len);
#ifdef __DEBUG
		printf ("got %d bytes\n",len);
#endif
		input += len;
                len = IO_read(BUF,256,mSec*1000);
        }
        if (input != 0){
#ifdef __DEBUG
                printf("\x1B[33m> "); printInHex(INPUT, input + len);
                printf("\n input = %d\n", input);
#endif
			valid_parse(INPUT,input);
        }
}

int XB_Checksum(unsigned char* mem){
        unsigned char total = 0;
        int len;
        int i;

        if (mem != NULL){
                len =  mem[1]*256 + mem[2] + 3;

                for (i = 3; i < len; i++){
                        total += mem[i];
                }
                mem[i] = 0xff - total;
        }
}

int XB_AT(int cnt, char* cmd, char* data, unsigned char len){
        char packet[256]={0x7e, 0 , 4+len,0x08,0x01+cnt,cmd[0],cmd[1]};
        if (len > 0){
                memcpy(&packet[7],data,len);
        }
        XB_Checksum(packet);
#ifdef __DEBUG
        printf("\x1B[36m< "); printInHex(packet,len+8); printf("\n");
#endif
        IO_write(packet, len + 8);
        listen(5000);
}

void XB_TX(int cnt, char*data, size_t len){
        char packet[256] = {    0x7e,   0,      14+len,         0x10,           cnt+1,
                                addr64[0],      addr64[1],      addr64[2],      addr64[3],
                                addr64[4],      addr64[5],      addr64[6],      addr64[7],
                                addr16[0],      addr16[1],      0,      0};


        memcpy(&packet[17], data, len);
        len += 18;
        XB_Checksum(packet);
#ifdef __DEBUG
        printf ("\x1B[36m< ");printInHex(packet,len);printf("\n");
#endif
        IO_write(packet,len);
        listen(3000);
//        printf ("\x1B[36m< ");printInHex(RX_data.data,RX_data.len);printf("\n");
	
}

