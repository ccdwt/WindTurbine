#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//internal  DateTime, 
//weather Barometer, OutSideTemp, WindSpeed, tenMinAgvWind, WindDir, OutsideHumidity, RainRate, SolarRad, DayRain, DayET, UVIndex
//turbine_sip ,PowerOutKW, RPM
//turbine_pwr PwrMeterkWTotalReal, PwrMeterkWhImport, PwrMeterkWhExport, PwrMeterkWhTotal, PwrMeterkWhNet
//rowland_pwr RowlandkWTotalReal, RowlandkWhImport, RowlandkWhExport, RowlandkWhTotal, RowlandkWhNet

char fields [ 1024 ];
char values [ 2048 ];

int csv(char* input, char ** argv);
char * previous_line(FILE* fd, char * line);

// breaking down 
//
//
//



void append (char * orig, char* piece){
	int end = strlen(orig);
	int len = strlen(piece);
	memcpy(&orig[end], piece, len);
	printf("%s\n", orig);
}

int function(int argc, char ** argv){
	fields[0]=0;
	values[0]=0;
	if (argc == 3){
		append (fields, argv[1]);
		append (values, argv[2]);
	} else {
		printf("i'm yelling at you \n");
		return (1);
	} 
	char cmd[2048];
	printf("INSERT INTO Table1 (%s) VALUES (%s);\n",fields,values);
	sprintf(cmd,"INSERT INTO Table1 (%s) VALUES (%s);",fields,values);
	printf ("%s\n",cmd);
}
	

int main(){
	FILE * fd;
	char *filename = "test.csv";
	char buf[1024];
	char * fields[20];
	if ((fd = fopen(filename, "rb")) != NULL){
		int count;
		for (count = 0; count < 3; count ++){

			char * line = previous_line(fd,buf);
			printf("%s\n",line);
			int i;
			int argc = csv(line,fields);
			for(i=0; i < argc; i++){
				printf("field %d: %s\n",i,fields[i]);
			}
		}
		fclose(fd);
	}
}

char * previous_line(FILE * fd, char* line){
	char * last_newline = NULL;
	size_t len;
	static struct{
		FILE * fd;
		int length; // guessed length of line.
	} state;

	if (fd != state.fd){
		state.fd = fd;
		state.length = 0;
	}
	if (state.length == 0){ 
		state.length = 10; // 260
		fseek(state.fd,0,SEEK_END);
	}
	//printf("ftell before seek = %ld\n",ftell(state.fd));

	if (fseek(state.fd, -1 * (state.length), SEEK_CUR) == -1){
		state.length = ftell(state.fd);
		fseek(state.fd,0,SEEK_SET);
	}
	while (last_newline == NULL){
		printf("ftell = %ld\n", ftell(state.fd));
		len = fread(line,1,state.length,fd);
		fseek(state.fd, -1 * len, SEEK_CUR); // go back to where we were.
		//printf ("got %u bytes\n",len);
		line[len] = '\0';
		//printf("line was : %s \n",line);
		last_newline = strrchr(line,'\n');
		if (last_newline == &line[len-1]){ 
			state.length --;
			line[--len]='\0';
			last_newline = strrchr(line,'\n');
		}

		//printf("last_newline returns %p %p\n",last_newline, &line[len]);
		if (last_newline == NULL){
			state.length += 10;
			fseek(state.fd, -10,SEEK_CUR);
		}
	}

	state.length = ((const char *) &line[len-1] - (const char * )last_newline);
	fseek(state.fd, len-state.length, SEEK_CUR); // there are characters we didn't keep
	//printf("length was %d\n",state.length);

	return last_newline+1;

}

int csv(char* input, char ** argv){
	int argc = 1;

	argv[0] = strtok(input,",");

	while ((argv[argc++] = strtok(NULL,","))!=NULL);
	return argc-1;
}
