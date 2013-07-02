#ifndef __SIP_H__
#define __SIP_H__


#define bit(x)  (1 << (x))
#include <stdint.h>
#include <stdbool.h>
#include "SIP_Event_Codes.h"

typedef struct {
	uint16_t	SoftwareRev;	//		The inverter Software revision
	uint16_t	OPVersion;	//		The Version of this list of values
	uint32_t	Time;		// s		The inverter time
	int32_t		AccWh;		// W		Accumulated Watt Hours
	uint16_t	Vin;		// 0.1V		The input Voltage
	uint16_t	Vdcl;		// 0.1V		The inverter DC bus voltage
	uint16_t	Vline1;		// 0.1V		The line1 voltage
        uint16_t        Vline2;		// 0.1V		The line2 voltage
        uint16_t        Vrise;		// 0.1V		The voltage rise
        uint16_t        VfrRPM;		// 0.1V		Min input voltage from RPM
        uint16_t        Iout;		// 0.01A	The current output
        int16_t		Pout;		// W		The power output
        uint16_t        Preq;		// W		Requested Power
        uint16_t        Pmax;		// W		Inverter nominal output power
        uint16_t        LineFreq;	// 0.01Hz	The inverter line frequency
        uint16_t        InvFreq;	// 0.01HZ	The inverter frequency
        uint16_t        CalcRline;	// 		Calculated Line Resistance
        uint16_t        AltRPM;		// RPM		The alternator RPM
        uint16_t        WindSpeed;	// m/s		The calculated windspeed
        uint16_t        TargetTSR;	// 0.01		The target tip speed ratio
        uint16_t    	RampStartRPM;	// RPM		
        uint16_t        Bpw;		// 		The boost pulse width
        uint16_t        LimBpw;		// 		The maximum boost pulse width
        uint16_t        InvAmp;		// 		Inverter current amplitude
        uint16_t        ThsMaster;	// 0.1°C	The master processor heat
	uint16_t	ThsSlave;	// 0.1°C	The slave processor heat
        uint16_t        Thx;		// 0.1°C	The external thermistor reading
        uint16_t        EventCount;	// 		The number of events logged
        uint16_t        LastEventCode;	// 		The last event logged
        uint16_t        EventStatus;	// 		The event status
        uint16_t        EventValue;	// 		
        uint16_t        TurbineStatus;	// bitmap	The state of the turbine 
        uint16_t        GridStatus;	// bitmap	The state of the grid 
        uint16_t        SysStatus;	// bitmap	The state of the System 
        uint16_t        SlaveStatus;	// 		The state of the slave processor
        uint16_t        Access;		// 		The access level
        uint16_t        Timer;		// 		The timer (for counting down after shutdown events ..)
} SIP_Raw;

typedef struct {
	uint16_t	SoftwareRev;	//		The inverter Software revision
	uint16_t	OPVersion;	//		The Version of this list of values
	uint32_t	Time;		// s		The inverter time
	int32_t		AccWh;		// W		Accumulated Watt Hours
	double		Vin;		// 0.1V		The input Voltage
	double		Vdcl;		// 0.1V		The inverter DC bus voltage
	double		Vline1;		// 0.1V		The line1 voltage
        double	        Vline2;		// 0.1V		The line2 voltage
        double	        Vrise;		// 0.1V		The voltage rise
        double	        VfrRPM;		// 0.1V		Min input voltage from RPM
        double	        Iout;		// 0.01A	The current output
        int16_t	        Pout;		// W		The power output
        int16_t         Preq;		// W		Requested Power
        int16_t         Pmax;		// W		Inverter nominal output power
        double	        LineFreq;	// 0.01Hz	The inverter line frequency
        double	        InvFreq;	// 0.01HZ	The inverter frequency
        uint16_t        CalcRline;	// 		Calculated Line Resistance
        uint16_t        AltRPM;		// RPM		The alternator RPM
        uint16_t        WindSpeed;	// m/s		The calculated windspeed
        double		TargetTSR;	// 0.01		The target tip speed ratio
        uint16_t        RampStartRPM;	// RPM		
        uint16_t        Bpw;		// 		The boost pulse width
        uint16_t        LimBpw;		// 		The maximum boost pulse width
        uint16_t        InvAmp;		// 		Inverter current amplitude
        double		ThsMaster;	// 0.1°C	The master processor heat
	double		ThsSlave;	// 0.1°C	The slave processor heat
        double		Thx;		// 0.1°C	The external thermistor reading
        uint16_t        EventCount;	// 		The number of events logged
        uint16_t        LastEventCode;	// 		The last event logged
        uint16_t        EventStatus;	// 		The event status
        uint16_t        EventValue;	// 		
        uint16_t        TurbineStatus;	// bitmap	The state of the turbine 
        uint16_t        GridStatus;	// bitmap	The state of the grid 
        uint16_t        SysStatus;	// bitmap	The state of the System 
        uint16_t        SlaveStatus;	// 		The state of the slave processor
        uint16_t        Access;		// 		The access level
        uint16_t        Timer;		// 		The timer (for counting down after shutdown events ..)
} SIP_Engr;

/*System Status*/
#define ST_HS_BACKOFF 	0x0002 // High speed Backoff
#define ST_TX_LEN     	0x0004 // SIP Tx too loong
#define ST_TRAP		0x0008 // Improper Rst
#define ST_BATT_TO	0x0010 // Batt Timeout
#define ST_DRIVE_OFF	0x0020 // Drive Off
#define ST_SLAVE_SHUT	0x0040 // Slave Shutdown
#define ST_THSW_SHUT	0x0080 // Temp Shutdown
#define ST_RUN		0x0100 // Run
#define ST_DISABLED	0x0200 // Disabled
#define ST_WAITING	0x0400 // Waiting
#define ST_ADDR_FLAG	0x0800 // 
#define ST_HITEMP	0x1000 // High Temp
#define ST_BACKOFF	0x2000 // Temp Backoff
#define ST_SETPOINT_MAGIC	0x4000 // Bad Setpoints
#define ST_SETPOINT_CRC	0x8000 // Bad CRC

/*Turbine Status*/
#define TS_LOWSPEED	0X0001 // Law Windspeed
#define TS_BRAKING	0X0002 // Braking
#define TS_OVERSPEED	0X0004 // Overspeed
#define TS_NOSTALL	0X0010 // Spinup
#define TS_HIWIND_TEST	0X0020 // No Stall
#define TS_HIWIND_SHUT	0X0040 // High Wind Test
#define TS_RAMP		0X0100 // Ramp
#define TS_TSR_INCR	0X0200 // TSR Incr
#define TS_POWER_HIGH	0X0400 // Power High
#define TS_TSR_LIMIT	0X8000 // Tsr Limit
#define TS_QUIET	0X1000 // Quiet
#define TS_INCR_DELAY	0X2000 // Incr Delay
#define TS_RPM_CONTROL	0X4000 // Rpm Control

/*Grid Status*/
#define LST_L1V_LOW	0X0001 // L1 Low Voltage
#define LST_L1V_HIGH	0X0002 // L1 High Voltage
#define LST_L2V_LOW	0X0004 // L2 Low Voltage
#define LST_L2V_HIGH	0X0008 // L2 High Voltage
#define LST_L3V_LOW	0X0010 // 
#define LST_L3V_HIGH	0X0020 // 
#define LST_UNUSED	0X0040 // ?Offset Limit?
#define LST_PHASE_ERROR	0X0080 // Phase Error
#define LST_FREQ_LOW	0X0100 // Frequency Low
#define LST_FREQ_HIGH	0X0200 // Frequency High
#define LST_DPLL_UNLOCK	0X0400 // DPll Unlick
#define LST_H		0X0800 // 
#define LST_RELAY_OPEN	0X1000 // Grid Disconnect
#define LST_VAI		0X2000 // Anti Islanding?

extern SIP_Engr  Turbine_Status;
 
/*****
 * Given an integer and a number of bits, this function will return a reflection
 * of the integer in terms of bits.  For example, given an integer and a number
 * of 16 bits to reflect, this funcion will swap the high and low bytes in the
 * integer.  No function calls are made from this function.  This function is
 * called by calc_crc16(...) to swap the high and low bytes in its calculated value.
 *****/
unsigned int 	reflect		(unsigned int	v,	int b			);

/*****
 * Given a pointer and a number of bytes, this function will calculate and return
 * the crc16 value.  The CRC polynomial is 0x8005 and the crc register starts with
 * an initial value of 0.
 *****/
unsigned int 	calc_crc16	(unsigned char*	ch, 	unsigned int num_bytes	);
//unsigned char	flip_byte	(unsigned char	byte				);
//void 		flip		(unsigned char*	mem, 	int 		length	);
int		SIP_Request	(int		cnt,	char*		Packet	);
bool		SIP_Parse	(unsigned char*	mem,	unsigned int	len	);

#endif
