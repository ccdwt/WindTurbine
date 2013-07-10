#include "XB.h"
#include "io.h"
#include <string.h>

void er(char * m){
	fprintf(stderr,"error: %s\n", m);
}

void usage(){

}

int main(int argc, char*argv[]){
	char * tty = NULL;
	char * cmd = NULL;
	char data[100];
	int dataLen = 0;
	if (argc >= 3){
		tty = argv[1];
		cmd = argv[2];
		if (argc == 4){
			int i;
			char c[2];
			for (i = 0; i < strlen(argv[3]); i+=2){
				c[0] = argv[3][i];
				if (strlen(argv[3]) > i){
					c[1] = argv[3][i+1];
				} else {
					c[1] = '\0';
				}
				data[dataLen++] = (unsigned char) strtol(c,NULL,16);
			}
		} else if (argc > 4){
			er("too many arguments");
			usage();
			return 1;
		}
	} else {
		er("not enough arguments");
		usage();
		return 1;
	}
#ifdef __WIND_SERIAL_H__
	serial_port.name = tty;
	serial_port.baud = B115200;
#endif
	IO_init();
	printf ("XB_AT(0,\"%s\",",cmd);(data==NULL?printf("NULL"):printInHex(data,dataLen));printf(",%u);\n",dataLen);
	XB_AT(0,cmd,data,dataLen);
	IO_close();

}
