include make.inc

all: build

build: weather turbine modbus

test: turbine/turbine modbus/test weather/test

clean: weather/clean turbine/clean modbus/clean BeagleBone/clean db/clean
	rm -rf bin/* *~ *.swp

######### install  #####################
install: install_BBB install_noarch

install_BBB: warning
	+$(MAKE) install -C BeagleBone

install_noarch: install_loggers install_db

install_loggers: loggers install_modbus
#	@echo "installing weather_log"
#	@cp weather/log $(BIN)/weather_log
	@echo "installing turbine_log"
	@cp turbine/logger $(BIN)/turbine_log

install_test: tests
	@echo "installing weather_test"
	@cp weather/test $(BIN)/weather_test
	@echo "installing turbine_test"
	@cp turbine/turbine $(BIN)/turbine_test
	@echo "installing power_test"
	@cp modbus/test.pl$(BIN)/power_test
	@cp modbus/config.xml $(BIN)/config.xml
	
install_db: db/install_db db/install_cron

install_modbus: 
	@$(MAKE) install_log -C modbus

install_weather:
	@$(MAKE) install_log -C weather

warning: scripts/warn.sh
	scripts/warn.sh
	
loggers: weather/log turbine/logger modbus/log

tests: weather/test turbine/turbine modbus/test
	
######### weather/ #####################
weather: weather/test weather/log
	+$(MAKE) -C weather

weather/test:
	+$(MAKE) test -C weather

weather/log:
	+$(MAKE) log -C weather

weather/clean:
	@$(MAKE) clean -C weather
					
######## turbine/ ######################

turbine/util: turbine/asoc turbine/at

turbine: force
	+$(MAKE) -C turbine

turbine/turbine: force
	+$(MAKE) turbine -C turbine

turbine/logger: force
	+$(MAKE) logger -C turbine

turbine/asoc: force
	+$(MAKE) asoc -C turbine

turbine/at: force
	+$(MAKE) at -C turbine

turbine/clean: force
	@$(MAKE) clean -C turbine
######## modbus/ #######################

modbus: modbus/test modbus/log

modbus/test: force
	+$(MAKE) -C modbus

modbus/log: force
	+$(MAKE) log -C modbus

modbus/clean: force
	+$(MAKE) clean -C modbus
############## DB ######################

db/write: force
	+$(MAKE) write -C db

db/install_db: force
	+$(MAKE) install_db -C db

db/install_cron: force
	+$(MAKE) install_cron -C db

db/clean: force
	+$(MAKE) clean -C db

######## BeagleBone ####################
BeagleBone/clean: force
	+$(MAKE) clean -C BeagleBone


force:
	true
