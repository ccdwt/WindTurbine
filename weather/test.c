/*
** client.c -- a stream socket client demo
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include "weather.h"

#define PORT "22222" // the port client will be connecting to 
#define MAXDATASIZE 500 // max number of bytes we can get at once 



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
    
    timeout.tv_sec = 1; // default timeout 2sec;
    timeout.tv_usec = 0; //default timeout 2.00 sec;

    while(send(sockfd,data,len,0) == -1);

    ret.error = select(sockfd+1,&set,NULL,NULL,&timeout);
    if (ret.error > 0){
 	ret.len = read(sockfd,ret.data,MAXDATASIZE-1);

	//if ((ret.len = recv(sockfd, ret.data, MAXDATASIZE-1,0)) == -1) {
        //	ret.error=-1;
	//	printf("we got a real error");
        //}
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

    do{
    	ret = write_read("VER\n",4,sockfd);
    }while (ret.error <= 0);
    //printf("appeared to have gotten the version. packet length %d\n",ret.len);
    memcpy (version,ret.data,ret.len);
    version[ret.len] = '\0';
    //printf("version is %s",version);
    //printf("sending cmd %s",cmd1);

    do{
	ret = write_read(cmd1,7,sockfd);
        //printf("code is %d\n",ret.error);
    }while(ret.error<=0);

    //printf("recieved %d bytes\n",ret.len);
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
	
int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    char version[50];
    hilow_parse hilow;
    loop_parse loop;// __attribute__ ((packed));
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
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
    //printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure


/***	HILOWS	***/
    buffer data = get_weather("HILOWS",sockfd);
    find_start(&data);
    memcpy(&hilow,data.data,data.len);
    printf("daily high wind speed = %umph\n", hilow.wind.day);
    printf("daily high temperature = %.2f F\n",((float)hilow.outTemp.dayHigh/10));
    printf("daily low DP = %d F\n",hilow.dewPoint.dayLow);
    printf("mothly high Humididty = %u%%\n", hilow.extraHums.monthHigh[0]);
    printf("daily Low Humidity = %u%%\n", hilow.extraHums.dayLow[0]);
    data = get_weather("LOOP 1",sockfd);
    find_start(&data);
    memcpy(&loop,data.data,data.len);
    //printInHex(data.data,data.len);printf("\n");

//    printf("id\t\t\t= %c%c%c\n",loop.id[0], loop.id[1], loop.id[2]);
//    printf("barTrend\t\t= %u\n", loop.barTrend);
//    printf("Packet type\t\t= %u\n",(loop.packetType));
//    printf("NextRecord\t\t= %u\n",loop.nextRecord);
    printf("barometric pressure\t= %.3f in. Hg\n",(float)loop.barometer/1000);
//    printf("Inside Temperature\t= %.2f F %.2f C\n",((float)loop.insideTemperature/10), F2C((float)loop.insideTemperature/10));
//    printf("Inside Humidity\t\t= %d%%\n",loop.insideHumidity);
    printf("Outside Temperature\t= %.2f F %.2f C\n", ((float)loop.outsideTemperature/10),F2C((float)loop.outsideTemperature/10));
    printf("Wind Speed\t\t= %u mph\n", loop.windSpeed);
    printf("10 Min Avg Wind Speed\t= %d mph\n",loop.tenMinAvgWS);
    printf("Wind Direction\t\t= %u degrees\n", loop.windDirection);
    printf("outside Humidity \t= %d%%\n",loop.outsideHumidity);
    printf("Rain Rate\t\t= %.2f in. per hour\n",((float)loop.rainRate/100));
    printf("Solar Radiation\t\t= %u watts/m^2\n",loop.solarRadiation);
    printf("Day Rain \t\t= %.2f in.\n",((float)loop.dayRain/100)); 
    printf("Month Rain \t\t= %.2f in.\n",((float)loop.monthRain/100)); 
    printf("Year Rain \t\t= %.2f in.\n",((float)loop.yearRain/100)); 
    printf("Day ET \t\t\t= %.2f in.\n",((float)loop.dayET/100)); 
    printf("Month ET \t\t= %.2f in.\n",((float)loop.monthET/100)); 
    printf("Year ET \t\t= %.2f in.\n",((float)loop.yearET/100)); 
    printf("sunrise was at \t\t  %d:%.2d\n", loop.timeOfSunrise/100, loop.timeOfSunrise %100);
    printf("sunset was at \t\t  %d:%.2d\n",loop.timeOfSunset/100,loop.timeOfSunset %100);
    close(sockfd);

    return 0;
}

