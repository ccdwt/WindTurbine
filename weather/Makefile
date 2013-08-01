include ../make.inc

all: test log

log: log.c weather.h
	gcc $(CCFLAGS) $(CONFIG_DEF) $(LOG_DEF) $(PID_DEF) $(WEATHER_HOST_DEF) $(WEATHER_INTERVAL_DEF) -o log log.c

test: test.c weather.h
	gcc $(CCFLAGS) -o test test.c

weather: startup.m4
	m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) startup.m4 > weather

install_log:
	@echo "Installing binary"
	@$(CP) log $(BIN)/weather_log
	@echo "Installing startup script"
	@$(CP) weather /etc/init.d/weather	

clean: 
	rm -rf test test.exe log log.exe weather *.o *~ *.csv 
