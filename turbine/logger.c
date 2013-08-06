#include <stddef.h>
#include "SIP.h"
#include "XB.h"
#include "io.h"
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#ifndef PIDDIR
#define PIDDIR "/usr/src/WindTurbine/pids"
#endif

#ifndef LOGDIR
#define LOGDIR "/usr/src/WindTurbine/logs"
#endif

#ifndef SERIAL
#define SERIAL "/dev/ttyO1"
#endif

int usage(char* message, char * me){
	int ret = 0;
	if (message != NULL){
		ret = 1;
		printf("error: %s\n",message);
		
	}
	printf("%s [logfile]\n", me);
	return ret;
}

int main(int argc, char *argv[]) {
	
	char * filename = "TurbineLog";
	char line[15];
	pid_t pid;
	char SIP[15];
	int wait = 20; // log every 5 seconds;
	int cnt;
	int input;
	time_t timer;
	ssize_t len;
	char BUF[256];
	if (argc == 2){
		filename = argv[1];
	} else if ( argc > 2 ){
		return usage("Too many Arguments",argv[0]);
	}
	pid = fork();
	if (pid < 0){
		exit(2);
	}
	if (pid > 0){
		// sucessful fork;
		FILE * pidFile = fopen(PIDDIR"/turbine_log.pid", "wt");
		printf("started daemon with pid: %d\n", pid);
		fprintf(pidFile,"%d\n", pid);
		exit(0);
	}
	// we are the child if we got here
	
	FILE *log = fopen(filename,"wt");
	stderr = freopen(LOGDIR"/turbine_error.log", "wt", stderr);
#ifndef DEBUG
	close(STDOUT_FILENO);
#endif
	close(STDIN_FILENO);


#ifdef __WIND_SERIAL_H__
	serial_port.name = SERIAL;
	serial_port.baud = B115200;
#endif
	IO_init();

	FILE * pidFile = fopen(PIDDIR"/turbine_log.pid", "r");
	fread(line,1,15,pidFile);
	pid = atoi(line);
	fprintf(stderr, "pid was %d\n", pid);
	fflush(stderr);
	fclose(pidFile);

	time(&timer);
	while (pid == getpid()){
		for (cnt = 0; cnt < 64; cnt ++){ 
			if (pid != getpid()) break;
			if ( time(NULL) == timer){
				struct timeval t;
				gettimeofday(&t,NULL);
				int interval = (wait - (t.tv_sec%wait) )*1000000 - t.tv_usec;
				interval = interval > 0? interval:0;
#ifdef DEBUG
 				fprintf(stderr,"sleeping %lf\n",(double)interval/1000000);
				fflush(stderr);
#endif
				usleep(interval ); 
			}
			//while((time(NULL) - timer == 0)||(time(NULL) % wait !=0));
			int len = SIP_Request(cnt,SIP);
			if (time(NULL) -timer>= wait*2){
#ifdef DEBUG
				fprintf(stderr,"doing a io restart @ %u\n",time(NULL));
#endif 
				IO_close();
				IO_init();
				time(&timer); timer -= wait; // wait a little to do the next io restart.
			}
			XB_TX(cnt, SIP,len);
			if ((RX_data.len != 0) && (SIP_Parse(RX_data.data, RX_data.len) )) {
				time(&timer); // record successful packet
				fprintf(log,	"%lu",	 timer);			// 0
				fprintf(log,	",%u",	 Turbine_Status.SoftwareRev);	// 1
				fprintf(log,	",%u",	 Turbine_Status.OPVersion);	// 2
				fprintf(log,	",%lu",	 Turbine_Status.Time);		// 3
				fprintf(log,	",%lu",	 Turbine_Status.AccWh);		// 4
				fprintf(log,	",%.2lf",Turbine_Status.Vin);		// 5
				fprintf(log,	",%.2lf",Turbine_Status.Vdcl);		// 6
				fprintf(log,	",%.2lf",Turbine_Status.Vline1);	// 7
				fprintf(log,	",%.2lf",Turbine_Status.Vline2);	// 8
				fprintf(log,	",%.2lf",Turbine_Status.Vrise);		// 9
				fprintf(log,	",%.2lf",Turbine_Status.VfrRPM);	// 10
				fprintf(log,	",%.2lf",Turbine_Status.Iout);		// 11
				fprintf(log,	",%d",	 Turbine_Status.Pout);		// 12
				fprintf(log,	",%d",	 Turbine_Status.Preq);		// 13
				fprintf(log,	",%d",	 Turbine_Status.Pmax);		// 14
				fprintf(log,	",%.2lf",Turbine_Status.LineFreq);	// 15
				fprintf(log,	",%.2lf",Turbine_Status.InvFreq);	// 16
				fprintf(log,	",%u",	 Turbine_Status.CalcRline);	// 17
				fprintf(log,	",%u",	 Turbine_Status.AltRPM);	// 18
				fprintf(log,	",%u",	 Turbine_Status.WindSpeed);	// 19
				fprintf(log,	",%.2lf",Turbine_Status.TargetTSR);	// 20
				fprintf(log,	",%u",	 Turbine_Status.RampStartRPM);	// 21
				fprintf(log,	",%u",	 Turbine_Status.Bpw);		// 22
				fprintf(log,	",%u",	 Turbine_Status.LimBpw);	// 23
				fprintf(log,	",%u",	 Turbine_Status.InvAmp);	// 24
				fprintf(log,	",%.2lf",Turbine_Status.ThsMaster);	// 25
				fprintf(log,	",%.2lf",Turbine_Status.ThsSlave);	// 26
				fprintf(log,	",%.2lf",Turbine_Status.Thx);		// 27
				fprintf(log,	",%u",	 Turbine_Status.EventCount);	// 28
				fprintf(log,	",%u",	 Turbine_Status.LastEventCode);	// 29
				fprintf(log,	",%u",	 Turbine_Status.EventStatus);	// 30
				fprintf(log,	",%u",	 Turbine_Status.EventValue);	// 31
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_HS_BACKOFF));	// 32
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_TX_LEN));		// 33
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_TRAP));		// 34
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_BATT_TO));	// 35
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_DRIVE_OFF));	// 36
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SLAVE_SHUT));	// 37
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_THSW_SHUT));	// 38
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_RUN));		// 39
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_DISABLED));	// 40
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_WAITING));	// 41
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_ADDR_FLAG));	// 42
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_HITEMP));		// 43
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_BACKOFF));	// 44
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SETPOINT_MAGIC));	// 45
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SETPOINT_CRC));	// 46
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_LOWSPEED));	// 47
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_BRAKING));	// 48
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_OVERSPEED));	// 49
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_NOSTALL));	// 50
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_HIWIND_TEST));// 51
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_HIWIND_SHUT));// 52
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_RAMP));	// 53
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_TSR_INCR));	// 54
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_POWER_HIGH));	// 55
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_TSR_LIMIT));	// 56
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_QUIET));	// 57
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_INCR_DELAY));	// 58
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_RPM_CONTROL));// 59
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L1V_LOW));	// 60
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L1V_HIGH));	// 61
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L2V_LOW));	// 62
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L2V_HIGH));	// 63
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L3V_LOW));	// 64
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L3V_HIGH));	// 65
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_UNUSED));	// 66
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_PHASE_ERROR));	// 67
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_FREQ_LOW));	// 68
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_FREQ_HIGH));	// 69
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_DPLL_UNLOCK));	// 70
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_H));		// 71
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_RELAY_OPEN));	// 72
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_VAI));		// 73
				fprintf(log,	",%u",	 Turbine_Status.SlaveStatus);	// 74
				fprintf(log,	",%u",	 Turbine_Status.Access);	// 75
				fprintf(log,	",%u",	 Turbine_Status.Timer);		// 76
				fprintf(log,"\n");
				fflush(log);
			}
			RX_data.len = 0;
		}


		FILE * pidFile = fopen(PIDDIR"/turbine_log.pid", "r");
		fread(line,1,15,pidFile);
		pid = atoi(line);
		fprintf(stderr, "pid was %d\n", pid);
		fflush(stderr);
		fclose(pidFile);

	}
	IO_close();
	fclose(log);
}	
