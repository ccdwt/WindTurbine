/*
** client.c -- a stream socket client demo
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include "weather.h"

#define PORT "22222" // the port client will be connecting to 
#define MAXDATASIZE 500 // max number of bytes we can get at once 

int interval = 2; //interval in seconds;

void printInHex(char* arry, int len){
	int i =0;
	for (i=0;i<len;i++){
		printf("%3.2x",(unsigned char)arry[i]);
	}
}

float F2C(float F){ 
	return (F-32)/1.8;
} 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

buffer write_read(char*data,int len,int sockfd){
    fd_set set;
    struct timeval timeout;
    buffer ret;
    static char buf[MAXDATASIZE];
    ret.data = buf;

    ret.len = 0;
    ret.error = 0;

    FD_ZERO(&set);
    FD_SET(sockfd,&set);
    
    timeout.tv_sec = 1; // default timeout 1sec;
    timeout.tv_usec = 0; //default timeout 1.00 sec;

    if (fcntl(sockfd,F_GETFD) == -1 || errno ==EBADF){
    	ret.error = -2; // bad file descriptor
	return ret;
    }
    int i;
    while((ret.error = send(sockfd,data,len,MSG_NOSIGNAL)) < 0){
	printf("ret.error = %d\n",ret.error);
	if (i++ > 5){
		ret.error = -4;// cannot write
		break;
	}
    }
	if(ret.error < 0){
		return ret;
	}
    ret.error = select(sockfd+1,&set,NULL,NULL,&timeout);
    if (ret.error > 0){
 	ret.len = read(sockfd,ret.data,MAXDATASIZE-1);

    }
    //printf("length recieved is %d\n",ret.len);
    return ret;
}

     
buffer get_weather(char * cmd, int sockfd){
    int numbytes;
    char version[50];
    char cmd1[8];
    struct timeval timeout;
    buffer ret;

    memcpy (cmd1,cmd,6);
    cmd1[6]='\n';
    cmd1[7]='\0';

    printf("starting wakeup .."); fflush(stdout);
    do{
    	ret = write_read("VER\n",4,sockfd);
	if (ret.error <= -2 || ret.error == 4294967293 ){
		printf("Bad FD\n");
		if (ret.error == 4294967293 ) ret.error=-2;
		return ret;
	}
    }while (ret.error <= 0);
    printf("done\n");
    memcpy (version,ret.data,ret.len);
    version[ret.len] = '\0';
    //printf("version is %s",version);
    printf("sending cmd %s ",cmd1); fflush(stdout);

    do{
	ret = write_read(cmd1,7,sockfd);
        printf("code is %d\n",ret.error);
	if (ret.error <= -2){
		printf("Bad FD\n");
		return ret;
	}
    }while(ret.error<=0);

    printf("recieved %d bytes\n",ret.len);
    return ret;
}

void find_start(buffer *data){

    while(data->len > 0){
	data->data = &data->data[1];
	data->len --;
	if (data->data[-1] == '\6'){
		break;
	}
    }
}
	
int queryLog(char* hostname, FILE * log){
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    char version[50];
    hilow_parse hilow;
    loop_parse loop;// __attribute__ ((packed));
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure


/***	LOOP 1	***/
    buffer data; 
    data = get_weather("LOOP 1",sockfd);
    printf("got data from get_weather\n");
    if (data.error >= 0){
    	find_start(&data);
    	if (data.len >= sizeof(loop)){
		memcpy(&loop,data.data,data.len);
		fprintf(log,"%u,seconds",time(NULL));
		fprintf(log,",%.3f,in. Hg",(float)loop.barometer/1000);
		fprintf(log,",%.2f,\u00B0F", ((float)loop.outsideTemperature/10));
		fprintf(log,",%u,mph", loop.windSpeed);
		fprintf(log,",%d,mph",loop.tenMinAvgWS);
		fprintf(log,",%u,\u00B0", loop.windDirection);
		fprintf(log,",%d,%%",loop.outsideHumidity);
		fprintf(log,",%.2f,in./hr.",((float)loop.rainRate/100));
		fprintf(log,",%u,UV index",loop.uvIndex);
		fprintf(log,",%u,W/m\u00B2",loop.solarRadiation);
		fprintf(log,",%.2f,in.",((float)loop.dayRain/100)); 
		fprintf(log,",%.2f,in.",((float)loop.monthRain/100)); 
		fprintf(log,",%.2f,in.",((float)loop.yearRain/100)); 
		fprintf(log,",%.2f,in.",((float)loop.dayET/100)); 
		fprintf(log,",%.2f,in.",((float)loop.monthET/100)); 
		fprintf(log,",%.2f,in.",((float)loop.yearET/100)); 
		fprintf(log,",%u,batt",loop.transmitterBat);
		fprintf(log,",%.2f,V",(float)loop.consoleBattery/100);
		fprintf(log,"\n");
		fflush(log);
	} else {
		close(sockfd);
		return -3; // packet too short
	}
    } else  {
	    close(sockfd);
	    return data.error;
    }
    close(sockfd);

    return 0;
}


int main(int argc, char *argv[])
{
    char * filename = "log.csv";
    if (argc < 2 || argc > 3) {
        fprintf(stderr,"usage: %s hostname [logfile]\n", argv[0]);
        exit(1);
    }
    if (argc == 3){
	    filename = argv[2];
    }

    FILE * log = fopen(filename, "wt");
    while(1){
    printf("sleeping %u seconds\n", interval - (time(NULL) % interval));	    
    sleep (interval - (time(NULL)%interval));
    printf("getting info\n");
    printf("Log returned %u\n", queryLog(argv[1],log));
    }
    fclose(log);
}


