#ifndef _XB_H_
#define _XB_H_


typedef struct {
	size_t len;
	unsigned char type;
	unsigned char seq;
	unsigned char cmd[2];
	unsigned char addr64[8];
	unsigned char addr16[2];
	unsigned char opts;
	char data[1024];
} packet_t;

typedef struct{
	unsigned char data [2048];
	size_t len;
} buffer;
	
extern unsigned char addr64[8];
extern unsigned char addr16[2];
extern buffer RX_data;

void listen(unsigned int mSec);

int XB_Checksum(unsigned char* mem);

int XB_AT(int cnt, char* cmd, char* data, unsigned char len);

void XB_TX(int cnt, char* data, size_t len);
		
#endif
