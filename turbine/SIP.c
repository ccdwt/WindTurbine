#include "SIP.h"
#include "io.h"
#include <string.h>

SIP_Raw  Turbine_Raw;
SIP_Engr Turbine_Status;

#define bit(x)  (1 << (x))

/*****
 * Given an integer and a number of bits, this function will return a reflection
 * of the integer in terms of bits.  For example, given an integer and a number
 * of 16 bits to reflect, this funcion will swap the high and low bytes in the
 * integer.  No function calls are made from this function.  This function is
 * called by calc_crc16(...) to swap the high and low bytes in its calculated value.
 *****/
unsigned int reflect(unsigned int v,int b)
{
	int i;
	unsigned int temp;
	
	temp = v;
	v = 0x0000;
	for(i=0; i<b; i++){
		if(temp&0x0001)  v|=bit((b-1)-i);
		temp>>=1;
   }	
	return v;
}

/*****
 * Given a pointer and a number of bytes, this function will calculate and return
 * the crc16 value.  The CRC polynomial is 0x8005 and the crc register starts with
 * an initial value of 0.
 *****/
unsigned int calc_crc16(unsigned char *ch, unsigned int num_bytes)
{
	//Local variables
	int i;
	unsigned int uch, crcreg, currbyte;
	crcreg = 0;
	
	//Calculate the CRC16
	for(currbyte=0; currbyte<num_bytes; currbyte++) {
		uch = ch[currbyte];
		uch = reflect(uch,8);
		crcreg^=(uch<<8);
		for(i=0; i<8; i++) if(crcreg&0x8000) crcreg=(crcreg<<1)^0x8005; else crcreg<<=1;
   }
	
	//Swap the high and low bytes and return the value
	return (reflect(crcreg,16));
}

unsigned char flip_byte(unsigned char byte){
	int i;
	unsigned char out = 0;
	for (i = 1; i < 256; i*=2){
		out += byte % i;
	}
	return out;
}

void flip(unsigned char* mem, int length){
	int i;
	for (i = 0; i< length; i++){
		mem[i]=flip_byte(mem[i]);
	}
}

int SIP_Request(int cnt, char * Packet){
	char SIP[15] = {0x10,0x01,0x00,0x00,0x00,0x0a,cnt,0x00,0x05,0xd5+2*cnt,0x8a-5*cnt,0,0,0x10,0x03};
	unsigned int crc = calc_crc16(&SIP[2],9);
	SIP[11] = crc%256;
	SIP[12] = crc >> 8;
	memcpy(Packet,SIP,15);
	return (15);
}

bool SIP_Parse(unsigned char * mem, unsigned int len){
	unsigned int crc;
	if (mem != NULL && len > 80){
		crc = calc_crc16(&mem[2],len - 6);
	} else {
		return false;
	}
	if(	   mem[0] == 0x10	// DLE
		&& mem[1] == 0x02	// response
		&& mem[7] == 0x00	// correct command
		&& mem[8] == 0x05	// correct command
		&& mem[len-4] == crc % 256	//crc checksum
		&& mem[len-3] == crc >> 8	//crc checkusm
		&& mem[len-2] == 0x10	//DLE
		&& mem[len-1] == 0x03	//ETX
	   ){

		memcpy(&Turbine_Raw,&mem[11], sizeof(SIP_Raw)>len?len:sizeof(SIP_Raw));

		Turbine_Status.SoftwareRev   = 	Turbine_Raw.SoftwareRev;		
		Turbine_Status.OPVersion     = 	Turbine_Raw.OPVersion;		
		Turbine_Status.Time 	=	Turbine_Raw.Time;				
		Turbine_Status.AccWh 	=	Turbine_Raw.AccWh;				
		Turbine_Status.Vin 	=	(double)Turbine_Raw.Vin  	* 0.1;				
		Turbine_Status.Vdcl 	= 	(double)Turbine_Raw.Vdcl 	* 0.1;				
		Turbine_Status.Vline1 	= 	(double)Turbine_Raw.Vline1	* 0.1;				
		Turbine_Status.Vline2 	= 	(double)Turbine_Raw.Vline2	* 0.1;				
		Turbine_Status.Vrise 	= 	(double)Turbine_Raw.Vrise	* 0.1;				
		Turbine_Status.VfrRPM 	= 	(double)Turbine_Raw.VfrRPM	* 0.1;				
		Turbine_Status.Iout 	= 	(double)Turbine_Raw.Iout	* 0.01;
		Turbine_Status.Pout 	= 	Turbine_Raw.Pout;				
		Turbine_Status.Preq 	= 	Turbine_Raw.Preq;				
		Turbine_Status.Pmax 	= 	Turbine_Raw.Pmax;				
		Turbine_Status.LineFreq	= 	(double)Turbine_Raw.LineFreq	* 0.01;		
		Turbine_Status.InvFreq 	= 	(double)Turbine_Raw.InvFreq	* 0.01;	
		Turbine_Status.CalcRline= 	Turbine_Raw.CalcRline;		
		Turbine_Status.AltRPM 	= 	Turbine_Raw.AltRPM;				
		Turbine_Status.WindSpeed= 	Turbine_Raw.WindSpeed;		
		Turbine_Status.TargetTSR= 	(double)Turbine_Raw.TargetTSR	* 0.01;	
		Turbine_Status.RampStartRPM  = 	Turbine_Raw.RampStartRPM;		
		Turbine_Status.Bpw 	= 	Turbine_Raw.Bpw;				
		Turbine_Status.LimBpw 	= 	Turbine_Raw.LimBpw;				
		Turbine_Status.InvAmp 	= 	Turbine_Raw.InvAmp;				
		Turbine_Status.ThsMaster= 	(double)Turbine_Raw.ThsMaster	* 0.1;		
		Turbine_Status.ThsSlave	= 	(double)Turbine_Raw.ThsSlave	* 0.1;		
		Turbine_Status.Thx 	= 	(double)Turbine_Raw.Thx		* 0.1;				
		Turbine_Status.EventCount    = 	Turbine_Raw.EventCount;		
		Turbine_Status.LastEventCode = 	Turbine_Raw.LastEventCode;		
		Turbine_Status.EventStatus   = 	Turbine_Raw.EventStatus;		
		Turbine_Status.EventValue    = 	Turbine_Raw.EventValue;		
		Turbine_Status.TurbineStatus = 	Turbine_Raw.TurbineStatus;		
		Turbine_Status.GridStatus    = 	Turbine_Raw.GridStatus;		
		Turbine_Status.SysStatus     = 	Turbine_Raw.SysStatus;		
		Turbine_Status.SlaveStatus   = 	Turbine_Raw.SlaveStatus;		
		Turbine_Status.Access	= 	Turbine_Raw.Access;				
		Turbine_Status.Timer 	= 	Turbine_Raw.Timer;
	//	printf (" len = %5d  ", len);
		return true;
	}else {
// 		printf(" crc=%3.2x%3.2x, %3.2x%3.2x | len = %d sizeof struct = %d\n",crc%256,crc>>8,mem[len-4], mem[len-3], len, sizeof(Turbine_Raw));
		return false;
	}	
}
