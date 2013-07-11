#include <stddef.h>
#include "SIP.h"
#include "XB.h"
#include "io.h"
#include <time.h>
#include <sys/time.h>


int main(int argc, char *argv[]) {
	
	char * filename = "TurbineLog";
	
	char SIP[15];
	int wait = 20; // log every 5 seconds;
	int cnt;
	int input;
	time_t timer;
	ssize_t len;
	char BUF[256];
	FILE *log = fopen(filename,"wt");
#ifdef __WIND_SERIAL_H__
	serial_port.name = "/dev/ttyO1";
	serial_port.baud = B115200;
#endif
	IO_init();
	time(&timer);
	while (1){
		for (cnt = 0; cnt < 64; cnt ++){ 
			if ( time(NULL) == timer){
				struct timeval t;
				gettimeofday(&t,NULL);
				int interval = (wait - (t.tv_sec%wait) )*1000000 - t.tv_usec;
				interval = interval > 0? interval:0;
				printf("sleeping %lf\n",(double)interval/1000000);
				usleep(interval ); 
			}
			//while((time(NULL) - timer == 0)||(time(NULL) % wait !=0));
			int len = SIP_Request(cnt,SIP);
			XB_TX(cnt, SIP,len);
			if ((RX_data.len != 0) && (SIP_Parse(RX_data.data, RX_data.len) )) {
				time(&timer); // record successful packet
				fprintf(log,	"%lu",	 timer);
				fprintf(log,	",%u",	 Turbine_Status.SoftwareRev);
				fprintf(log,	",%u",	 Turbine_Status.OPVersion);
				fprintf(log,	",%lu",	 Turbine_Status.Time);
				fprintf(log,	",%lu",	 Turbine_Status.AccWh);
				fprintf(log,	",%.2lf",Turbine_Status.Vin);
				fprintf(log,	",%.2lf",Turbine_Status.Vdcl);
				fprintf(log,	",%.2lf",Turbine_Status.Vline1);
				fprintf(log,	",%.2lf",Turbine_Status.Vline2);
				fprintf(log,	",%.2lf",Turbine_Status.Vrise);
				fprintf(log,	",%.2lf",Turbine_Status.VfrRPM);
				fprintf(log,	",%.2lf",Turbine_Status.Iout);
				fprintf(log,	",%d",	 Turbine_Status.Pout);
				fprintf(log,	",%d",	 Turbine_Status.Preq);
				fprintf(log,	",%d",	 Turbine_Status.Pmax);
				fprintf(log,	",%.2lf",Turbine_Status.LineFreq);
				fprintf(log,	",%.2lf",Turbine_Status.InvFreq);
				fprintf(log,	",%u",	 Turbine_Status.CalcRline);
				fprintf(log,	",%u",	 Turbine_Status.AltRPM);
				fprintf(log,	",%u",	 Turbine_Status.WindSpeed);
				fprintf(log,	",%.2lf",Turbine_Status.TargetTSR);
				fprintf(log,	",%u",	 Turbine_Status.RampStartRPM);
				fprintf(log,	",%u",	 Turbine_Status.Bpw);
				fprintf(log,	",%u",	 Turbine_Status.LimBpw);
				fprintf(log,	",%u",	 Turbine_Status.InvAmp);
				fprintf(log,	",%.2lf",Turbine_Status.ThsMaster);
				fprintf(log,	",%.2lf",Turbine_Status.ThsSlave);
				fprintf(log,	",%.2lf",Turbine_Status.Thx);
				fprintf(log,	",%u",	 Turbine_Status.EventCount);
				fprintf(log,	",%u",	 Turbine_Status.LastEventCode);
				fprintf(log,	",%u",	 Turbine_Status.EventStatus);
				fprintf(log,	",%u",	 Turbine_Status.EventValue);
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_HS_BACKOFF));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_TX_LEN));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_TRAP));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_BATT_TO));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_DRIVE_OFF));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SLAVE_SHUT));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_THSW_SHUT));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_RUN));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_DISABLED));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_WAITING));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_ADDR_FLAG));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_HITEMP));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_BACKOFF));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SETPOINT_MAGIC));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.SysStatus & ST_SETPOINT_CRC));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_LOWSPEED));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_BRAKING));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_OVERSPEED));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_NOSTALL));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_HIWIND_TEST));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_HIWIND_SHUT));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_RAMP));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_TSR_INCR));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_POWER_HIGH));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_TSR_LIMIT));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_QUIET));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_INCR_DELAY));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.TurbineStatus & TS_RPM_CONTROL));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L1V_LOW));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L1V_HIGH));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L2V_LOW));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L2V_HIGH));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L3V_LOW));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_L3V_HIGH));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_UNUSED));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_PHASE_ERROR));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_FREQ_LOW));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_FREQ_HIGH));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_DPLL_UNLOCK));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_H));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_RELAY_OPEN));
				fprintf(log,	",%u",	 (bool)(Turbine_Status.GridStatus & LST_VAI));
				fprintf(log,	",%u",	 Turbine_Status.SlaveStatus);
				fprintf(log,	",%u",	 Turbine_Status.Access);
				fprintf(log,	",%u",	 Turbine_Status.Timer);
				fprintf(log,"\n");
				fflush(log);
			}
			RX_data.len = 0;
		}
	}
	IO_close();
	fclose(log);
}	
