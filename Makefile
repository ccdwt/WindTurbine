BIN="bin/"

all: build

build: weather turbine modbus

test: turbine/turbine modbus/test weather/test

clean: weather/clean turbine/clean modbus/clean BeagleBone/clean
	rm -rf bin/* *~ *.swp

######### install  #####################
install: install_BBB install_noarch

install_BBB: warning
	+$(MAKE) install -C BeagleBone

install_noarch: install_loggers install_db

install_loggers: loggers
	@echo "installing weather_log"
	@cp weather/test $(BIN)/weather_log
	@echo "installing turbine_log"
	@cp turbine/logger $(BIN)/turbine_log
	@echo "installing power_log"
	@cp modbus/log.pl $(BIN)/power_log
	@cp modbus/config.xml $(BIN)/config.xml

install_test: tests
	@echo "installing weather_test"
	@cp weather/test $(BIN)/weather_test
	@echo "installing turbine_test"
	@cp turbine/turbine $(BIN)/turbine_test
	@echo "installing power_test"
	@cp modbus/test.pl$(BIN)/power_test
	@cp modbus/config.xml $(BIN)/config.xml
	
install_db:
	@echo "install_db: not implemented yet"

warning: scripts/warn.sh
	scripts/warn.sh
	
loggers: weather/test turbine/logger modbus/log

tests: weather/test turbine/turbine modbus/test
	
######### weather/ #####################
weather: weather/test
	
weather/test:
	+$(MAKE) -C weather

weather/clean:
	@$(MAKE) clean -C weather
					
######## turbine/ ######################

turbine/util: turbine/asoc turbine/at

turbine:
	+$(MAKE) -C turbine

turbine/turbine:
	+$(MAKE) turbine -C turbine

turbine/logger:
	+$(MAKE) logger -C turbine

turbine/asoc:
	+$(MAKE) asoc -C turbine

turbine/at:
	+$(MAKE) at -C turbine

turbine/clean:
	@$(MAKE) clean -C turbine
######## modbus/ #######################

modbus: modbus/test modbus/log

modbus/test:
	+$(MAKE) -C modbus

modbus/log:
	+$(MAKE) log -C modbus

modbus/clean:
	+$(MAKE) clean -C modbus

######## BeagleBone ####################
BeagleBone/clean:
	+$(MAKE) clean -C BeagleBone
