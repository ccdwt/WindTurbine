#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef LOGDIR
#define LOGDIR "/usr/src/WindTurbine/logs"
#endif



char fields [ 1024 ];
char values [ 2048 ];
bool BOF; // beginning of file 

int csv(char* input, char ** argv);
int DBlog(char * filename, char * cmd);
static int callback(void *NotUsed, int argc, char ** argv, char **azColName);
char * previous_line(FILE* fd, char * line);
void turbine_logfile(long int now);
void weather_logfile(long int now);
void power_logfile(long int now, int which); // 0 = turbine, 1 = rowland

void kill_other_instances(char * name){
	pid_t me = getpid();
	char command[100];
	sprintf(command, "pidof \"%s\" -o %d | xargs -r kill ", name, me);
#ifdef DEBUG
	printf("command: %s\n", command);
#else
	system(command);
#endif


}
void append (char * orig, char* piece){
	int end = strlen(orig);
	int len = strlen(piece);
	memcpy(&orig[end], piece, len);
	//printf("%s\n", orig);
}

int main(int argc, char ** argv){
	char cmd[2048];
	char * db_file = DBDIR"/test.db" ;
	kill_other_instances(argv[0]);
	previous_line(NULL, NULL); // initialize values
	fields[0]=0;
	values[0]=0;
	if (argc == 2){
		db_file = argv[1];
	} 
	time_t now;
	time(&now);
	struct tm * local = localtime(&now);
	append(fields, "DateTime");
	sprintf(cmd, "\"%.4u-%.2u-%.2uT%.1u:%.2u:%.2u\"", local->tm_year+1900, local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	append(values, cmd);

	weather_logfile(now);
	turbine_logfile(now);
	power_logfile(now,0);
	power_logfile(now,1);
	if (fields[0] == ','){
		fields[0] = ' ';
	}
	if (values[0] == ','){
		values[0] = ' ';
	}
	sprintf(cmd,"INSERT INTO Table1 (%s) VALUES (%s);",fields,values);
#ifdef DEBUG
	printf ("%s\n",cmd);
#else
 	DBlog(db_file, cmd);
#endif

}
	


int DBlog(char * filename, char * cmd){
	sqlite3 *db;
	char * zErrMsg = 0;
	int rc;
	rc = sqlite3_open(filename, &db);
	if (rc){
		fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	rc = sqlite3_exec(db, cmd, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}

static int callback(void *NotUsed, int argc, char ** argv, char **azColName){
        int i;
        for (i = 0; i < argc; i++){
//                printf("%s=%s\n", azColName[i], argv[i] ? argv[i]: "NULL");
        }
        //printf("\n");
        return 0;
}

char * previous_line(FILE * fd, char* line){
	char * last_newline = NULL;
	size_t len;
	static struct{
		FILE * fd;
		int length; // guessed length of line.
	} state;

	if (fd == NULL){ // initialize state variable
		FILE* fd = NULL;
		int length = 0;
		return NULL;
	}

	if (fd != state.fd){
		state.fd = fd;
		state.length = 0;
	}
	if (state.length == 0){ 
		state.length = 100; // 260
		fseek(state.fd,0,SEEK_END);
	}

#ifdef DEBUG
 	printf("ftell before seek = %ld\n",ftell(state.fd));
#endif

	if (fseek(state.fd, -1 * (state.length), SEEK_CUR) == -1){
		state.length = ftell(state.fd);
		fseek(state.fd,0,SEEK_SET);
		BOF=true;
	} else if (ftell(state.fd) == 0){
		BOF = true;
	} else {
		BOF = false;
	}
	while (last_newline == NULL && BOF == false){
#ifdef DEBUG
// 		printf("ftell = %ld\n", ftell(state.fd));
#endif
		len = fread(line,1,state.length,fd);
		fseek(state.fd, -1 * len, SEEK_CUR); // go back to where we were.
// 		printf ("got %u bytes\n",len);
		line[len] = '\0';
// 		printf("line was : %s \n",line);
		last_newline = strrchr(line,'\n');
		if (last_newline == &line[len-1]){ 
			state.length --;
			line[--len]='\0';
			last_newline = strrchr(line,'\n');
		}

// 		printf("last_newline returns %p %p\n",last_newline, &line[len]);
		if (last_newline == NULL){
			state.length += 10;
			fseek(state.fd, -10,SEEK_CUR);
		}
	}

	state.length = ((const char *) &line[len-1] - (const char * )last_newline);
	fseek(state.fd, len-state.length, SEEK_CUR); // there are characters we didn't keep
// 	printf("length was %d\n",state.length);

	return (last_newline == NULL) ? NULL : last_newline+1;

}

int csv(char* input, char ** argv){
	int argc = 1;

	argv[0] = strtok(input,",");

	while ((argv[argc++] = strtok(NULL,","))!=NULL);
	return argc-1;
}



void turbine_logfile(long int now){
	unsigned int RPM = 0;
	double PowerOutKw = 0;
	long int timestamp;
	char buf[1024];
	char * argv[78];
	FILE * fd;
#ifdef DEBUG
	printf (LOGDIR"/turbine_log.csv\n");
#endif
	if ((fd = fopen(LOGDIR"/turbine_log.csv", "rb")) != NULL){
		int count = 0;
		while(1){
			char * line = previous_line(fd,buf);
#ifdef DEBUG
 			printf ("line: %s\n", line);
 			printf ("got last line\n");
#endif
			if (line != NULL || ! BOF){
				int argc = csv(line,argv);
#ifdef DEBUG
 				printf ("got %d values from csv\n", argc);
#endif
				timestamp = atoi(argv[0]);
#ifdef DEBUG
 				printf ("%s, %s, %s\n", argv[0],argv[12],argv[18]);
	 			printf ("ts = %d, now = %d\n", timestamp, now);
#endif
			}
			if ( (now - timestamp ) >= 60 || BOF){
				if (count == 0) return;
				break;
			}
			RPM += (unsigned) atoi(argv[18]);
			PowerOutKw +=(float) atol(argv[12]) / 1000; // <- W to kW
#ifdef DEBUG
			printf ("got t=%u,rpm = %u, pout = %d\n", atol(argv[0]), atol(argv[18]), atol(argv[12]));
#endif
			count ++;
		}	
		RPM /= count;
		PowerOutKw /= count;
		append(fields, ",RPM,PowerOutKw");
		sprintf(buf, ",%u,%.3f",RPM, PowerOutKw);
		append(values, buf);

	}
}

//turbine_pwr PwrMeterkWTotalReal, PwrMeterkWhImport, PwrMeterkWhExport, PwrMeterkWhTotal, PwrMeterkWhNet
//rowland_pwr RowlandkWTotalReal, RowlandkWhImport, RowlandkWhExport, RowlandkWhTotal, RowlandkWhNet
void power_logfile(long int now, int which){ // 0 = turbine, 1 = rowland
	double kWTotalReal= 0;
	int kWhImport= 0;
	int kWhExport=0;
	int kWhTotal=0;
	int kWhNet=0;
	FILE * fd;
	char * filename = which==0?LOGDIR"/power_turbine.csv":LOGDIR"/power_rowland.csv";
	long int timestamp;
	char buf[1024];
	char * argv[19];
// 	printf("opening \'%s\'\n",filename);
	if ((fd = fopen(filename,"rb"))!=NULL){
		int count=0;
		while(1){
			char * line = previous_line(fd,buf);
			if ( line != NULL || ! BOF){
				int argc = csv(line, argv);
	//			printf("got %d values on line\n",argc);
#ifdef DEBUG
				printf("%s, %s, %s, %s, %s, %s\n",argv[0], argv[8],argv[13],argv[14],argv[15],argv[16]);
#endif
				timestamp = atoi(argv[0]);
			}
			if ((now - timestamp) >= 60 ||BOF){
				if (count == 0) return;
				break;
			}
			kWTotalReal += atof(argv[7]) + atof(argv[8]);
			kWhImport += atoi(argv[13]);
			kWhExport += atoi(argv[14]);
			kWhTotal += atoi(argv[15]);
			kWhNet += atoi(argv[16]);
			count ++;
		}
		kWTotalReal /= count;
		kWhImport /= count;
		kWhExport /= count;
		kWhTotal /= count;
		kWhNet /=count;
		if (which){ // rowland
			append(fields, ",RowlandkWTotalReal, RowlandkWhImport, RowlandkWhExport, RowlandkWhTotal, RowlandkWhNet");
		} else { // turbine
			append(fields, ",PwrMeterkWTotalReal, PwrMeterkWhImport, PwrMeterkWhExport, PwrMeterkWhTotal, PwrMeterkWhNet");
		}
		sprintf(buf, ",%.2f,%d,%d,%d,%d",kWTotalReal, kWhImport, kWhExport, kWhTotal, kWhNet);
		append(values,buf);

	}
}

//weather Barometer, OutSideTemp, WindSpeed, tenMinAgvWind, WindDir, OutsideHumidity, RainRate, SolarRad, DayRain, DayET, UVIndex
void weather_logfile(long int now){
	float barometer = 0;
	float outtemp = 0;
	unsigned int windSpeed = 0;
	int tenMinAvgWS= 0;
	unsigned int windDirection=0;
	int outsideHumidity=0;
	float rainRate=0;
	unsigned int solarRadiation=0;
	float dayRain=0;
	float dayET=0;
	float uvIndex=0;
	FILE * fd;
	long int timestamp;
	char buf[1024];
	char * argv[100];
	if ((fd = fopen(LOGDIR"/weather_log.csv", "rb")) != NULL){
		int count = 0;
		while (1){
			char * line = previous_line(fd, buf);
			if (line != NULL || !BOF){
				int argc = csv(line, argv);
				timestamp = atoi(argv[0]);
			}
			if ((now - timestamp) >= 60||BOF){
				if (count == 0) return;
				break;
			}
			barometer	+= atof(argv[2]);
		        outtemp 	+= atof(argv[4]);
			windSpeed	+= atoi(argv[6]);
			tenMinAvgWS	+= atoi(argv[8]);
			windDirection	+= atoi(argv[10]);
			outsideHumidity	+= atoi(argv[12]);
			rainRate	+= atof(argv[14]);
			uvIndex		+= atof(argv[16]);
			solarRadiation	+= atoi(argv[18]);
			dayRain		+= atof(argv[20]);
			dayET		+= atof(argv[26]);
			count ++;
#ifdef DEBUG
 			printf ("solarRadiation = %d, %d \n",atoi(argv[18]));
#endif
		}
	barometer	/= count;
        outtemp 	/= count;
        windSpeed	/= count;
        tenMinAvgWS	/= count;
        windDirection	/= count;
        outsideHumidity	/= count;
        rainRate	/= count;
        uvIndex		/= count;
        solarRadiation	/= count;
        dayRain		/= count;
        dayET		/= count;
#ifdef DEBUG
	printf ("count = %d, solarradiation = %d\n", count, solarRadiation);
#endif
	append(fields, ",Barometer,OutSideTemp,WindSpeed,tenMinAgvWind,WindDir,OutsideHumidity,RainRate,SolarRad,DayRain,DayET,UVIndex");
	sprintf(buf,",%.3f,%.2f,%u,%d,%u,%d,%.2f,%u,%.2f,%.2f,%.1f",barometer,outtemp,windSpeed,tenMinAvgWS,windDirection,outsideHumidity,rainRate,solarRadiation,dayRain,dayET,uvIndex);
	append(values,buf);
	}
}

