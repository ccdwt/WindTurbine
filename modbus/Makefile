include ../make.inc

all: log test

install: install_log install_test

test: test.pl config.xml deps
	@echo "checking sytax of test.pl"
	@perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml
	@echo "adding configuration to log"
	@$(SCRIPTS)/perl_constants.sh log.pl log "CONFIGDIR=>\"$(CONFIGDIR)\"" "LOGDIR=>\"$(LOGDIR)\"" "PIDDIR=>\"$(PIDDIR)\""
	@chmod +x log
	@echo "checking syntax of log"
	@perl -c log; ../scripts/fixLocale.sh log

pwr_rowland: pwr.m4
	@m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=rowland pwr.m4 > pwr_rowland

pwr_turbine: pwr.m4
	@m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=turbine pwr.m4 > pwr_turbine

install_log: log config.xml pwr_rowland pwr_turbine
	@echo "Installing binaries for modbus"
	@$(CP) log $(BIN)/rowland_power_log
	@$(CP) log $(BIN)/turbine_power_log
	@echo "installing configuration"
	@$(CP) config.xml $(CONFIGDIR)/config.xml
	@echo "installing startup scripts"
	@$(CP) pwr_rowland /etc/init.d/
	@$(CP) pwr_turbine /etc/init.d/

deps: 
	@ echo -n "checking dependencies for modbus .. "
	@ if perl -e "$(grep ^use log.pl)"; then echo "Present"; else \
		echo "Not present";\
		echo "installing XML::Parser";\
		echo "installing IO::Socket";\
		opkg install libxml-parser-perl perl-module-io-socket;\
		echo "installing Proc::Daemon";\
		perl -MCPAN -e "install Proc::Daemon";\
		fi

clean:
	rm -f *.swp *~ *.log *.csv log test pwr_rowland pwr_turbine

