include make.inc

all: build

build: weather turbine modbus

test: turbine/turbine modbus/test weather/test

clean: weather/clean turbine/clean modbus/clean BeagleBone/clean db/clean monit/clean dropBox/clean
	rm -rf *~ *.swp

#uninstall: weather/uninstall turbine/uninstall modbus/uninstall BeagleBone/uninstall db/uninstall monit/uninstall dropBox/uninstall

######### install  #####################
install: install_dirs install_BBB install_noarch install_complete

install_complete:
	@echo "Installation complete run /etc/init.d/monit start or reboot"

install_BBB: warning
	+$(MAKE) install -C BeagleBone

install_noarch: install_loggers install_db install_backup install_monit

install_loggers: loggers install_modbus install_weather install_turbine

install_test: tests
	@echo "installing weather_test"
	@cp weather/test $(BIN)/weather_test
	@echo "installing turbine_test"
	@cp turbine/turbine $(BIN)/turbine_test
	@echo "installing power_test"
	@cp modbus/test.pl$(BIN)/power_test
	@cp modbus/config.xml $(BIN)/config.xml
	
install_db: db/install_db db/install_cron

install_backup: dropBox/install

install_modbus: 
	@$(MAKE) install_log -C modbus

install_weather:
	@$(MAKE) install_log -C weather

install_turbine:
	@$(MAKE) install_log -C turbine

install_monit:
	@$(MAKE) install -C utilities/monit

install_dirs:
	@echo "creating install directories if they do not exist"
	@if [ ! -d "$(INSTALL_BASE)" ]; then mkdir $(INSTALL_BASE); fi
	@if [ ! -d "$(CONFIGDIR)" ]; then mkdir $(CONFIGDIR); fi
	@if [ ! -d "$(PIDDIR)" ]; then mkdir $(PIDDIR); fi
	@if [ ! -d "$(LOGDIR)" ]; then mkdir $(LOGDIR); fi
	@if [ ! -d "$(DBDIR)" ]; then mkdir $(DBDIR); fi
	@if [ ! -d "$(BIN)" ]; then mkdir $(BIN); fi


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
########################################

dropBox/install: force
	+$(MAKE) install -C utilities/dropbox

dropBox/clean: force
	+$(MAKE) clean -C utilities/dropbox


monit/clean: force
	+$(MAKE) clean -C utilities/monit

force:
	true
