#ifndef __WEATHER__H_
#define __WEATHER__H_

typedef struct{
	char     id[3];							// should be 'L','O','O'
	uint8_t  packetType;						// should be 0
	 int8_t  barTrend;						// -60,-20,0,20,60 indicate rising or falling
	uint16_t nextRecord		__attribute__ ((packed));	// used to see if a new record is ready
	uint16_t barometer		__attribute__ ((packed));	// in. Hg/1000 should be between 20 and 32.5in Hg
	 int16_t insideTemperature	__attribute__ ((packed));	// *.1F
	uint8_t  insideHumidity;					// *1%;
	 int16_t outsideTemperature	__attribute__ ((packed));	// *.1F
	uint8_t  windSpeed;						// in mph
	uint8_t  tenMinAvgWS;						// in mph
	uint16_t windDirection		__attribute__ ((packed));	// in degrees 0=North
	uint8_t  extraTemp1;						// -90F
	uint8_t  extraTemp2;						// -90F
	uint8_t  extraTemp3;						// -90F
	uint8_t  extraTemp4;						// -90F
	uint8_t  extraTemp5;						// -90F
	uint8_t  extraTemp6;						// -90F
	uint8_t  extraTemp7;						// -90F
	uint8_t  soilTemp1;						// -90F
	uint8_t  soilTemp2;						// -90F
	uint8_t  soilTemp3;						// -90F
	uint8_t  soilTemp4;						// -90F
	uint8_t  leafTemp1;						// -90F
	uint8_t  leafTemp2;						// -90F
	uint8_t  leafTemp3;						// -90F
	uint8_t  leafTemp4;						// -90F	       
	uint8_t	 outsideHumidity;					// *1%
	uint8_t	 extraHumidities1;					// *1%
	uint8_t	 extraHumidities2;					// *1%
	uint8_t	 extraHumidities3;					// *1%
	uint8_t	 extraHumidities4;					// *1%
	uint8_t	 extraHumidities5;					// *1%
	uint8_t	 extraHumidities6;					// *1%
	uint8_t	 extraHumidities7;					// *1%
	uint16_t rainRate		__attribute__ ((packed));	// *.01 in./hr.
	uint8_t	 uvIndex;						// in UV index?
	uint16_t solarRadiation		__attribute__ ((packed));	// watt/meter squared
	uint16_t stormRain		__attribute__ ((packed));	// *.01 in.
	uint16_t stormStart		__attribute__ ((packed));	// bits 15-12 are month 11-7 is day 6-0 is year+2000
	uint16_t dayRain		__attribute__ ((packed));	// *.01 in.
	uint16_t monthRain		__attribute__ ((packed));	// *.01 in.
	uint16_t yearRain		__attribute__ ((packed));	// *.01 in.
	uint16_t dayET			__attribute__ ((packed));	// *.01 in.
	uint16_t monthET		__attribute__ ((packed));	// *.01 in.
	uint16_t yearET			__attribute__ ((packed));	// *.01 in.
	uint8_t  soilMoistures[4];					// in centibar.	
	uint8_t  leafWetness[4];					// 0-15 15=wet 0=dry
	uint8_t  insideAlarms;						//
	uint8_t  rainAlarms;						//
	uint16_t  outsideAlarms		__attribute__ ((packed));	//
	uint8_t  extraTempHumAlarms[8];					//
	uint8_t  soilLeafAlarms[4];					//
	uint8_t  transmitterBat;				
	uint16_t consoleBattery		__attribute__ ((packed));		
	uint8_t  forcastIcons;				
	uint8_t  forcastRuleNumber;			
	uint16_t timeOfSunrise		__attribute__ ((packed));		
	uint16_t timeOfSunset		__attribute__ ((packed));		
	char     newline;				
	char  	 carriageReturn;				
	uint16_t crc			__attribute__ ((packed));			
} loop_parse;

typedef struct {
	char * data;
	size_t 	len;
	int 	error;
} buffer;

struct hl5{	//High Low 5 byte
	uint8_t day;
	uint16_t time	__attribute__ ((packed));
	uint8_t month;
	uint8_t year;
};

struct hl16{	// High Low 16 byte
	uint16_t dayLow		__attribute__ ((packed));
	uint16_t dayHigh	__attribute__ ((packed));
	uint16_t timeHigh	__attribute__ ((packed));
	uint16_t timeLow	__attribute__ ((packed));
	uint16_t monthHigh	__attribute__ ((packed));
	uint16_t monthLow	__attribute__ ((packed));
	uint16_t yearHigh	__attribute__ ((packed));
	uint16_t yearLow	__attribute__ ((packed));
};

struct hl10{	// High Low 10 byte
	uint16_t dayHigh	__attribute__ ((packed));
	uint16_t timeHigh	__attribute__ ((packed));
	uint16_t hourHigh	__attribute__ ((packed));
	uint16_t monthHigh	__attribute__ ((packed));
	uint16_t yearHigh	__attribute__ ((packed));
};

struct hl8{	// High Low 8byte
	uint16_t dayHigh	__attribute__ ((packed));
	uint16_t timeHigh	__attribute__ ((packed));
	uint16_t monthHigh	__attribute__ ((packed));
	uint16_t yearHigh	__attribute__ ((packed));
};

typedef struct{
	struct hl16 barometer;
	struct hl5 wind;
	struct hl16 insideTemp;
	struct hl10 insideHumidity;
	struct hl16 outTemp;
	struct hl16 dewPoint;
	struct hl8 windChill;
	struct hl8 heatIndex;
	struct hl8 thswIndex;
	struct hl8 solarRadiation;
	struct hl5 UV;			
	struct hl10 rainRate;		
	char extraTemps[150];	
	char extraHums[80];		// replace
	char soilMoisture[40];
	char leafWetness[40];
	char crc[2];
} hilow_parse;

#endif
