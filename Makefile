BIN="bin/"

all: build

build: weather turbine 
#modbus

clean: weather/clean turbine/clean
	rm -rf bin/*

######### install  #####################
install: install_BBB install_noarch

install_BBB: warning

install_noarch: install_loggers install_db

install_loggers: loggers
	@echo "installing weather_log"
	@cp weather/client $(BIN)/weather_log
	@echo "installing turbine_log"
	@cp turbine/logger $(BIN)/turbine_log
#	cp modbus/logger $(BIN)/power_log
	
install_db:
	@echo "install_db: not implemented yet"

warning: scripts/warn.sh
	scripts/warn.sh
	
loggers: weather/client turbine/logger
	
######### weather/ #####################
weather: weather/client
	
weather/client:
	+$(MAKE) -C weather

weather/clean:
	@$(MAKE) clean -C weather
					
######## turbine/ ######################

turbine: turbine/turbine turbine/logger

turbine/turbine:
	+$(MAKE) -C turbine

turbine/logger:
	+$(MAKE) logger -C turbine

turbine/clean:
	@$(MAKE) clean -C turbine
######## modbus/ #######################
