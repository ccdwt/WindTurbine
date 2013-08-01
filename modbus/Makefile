include ../make.inc

all: log test

install: install_log install_test

test: test.pl config.xml
	perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml
	$(SCRIPTS)/perl_constants.sh log.pl log "CONFIGDIR=>\"$(CONFIGDIR)\"" "LOGDIR=>\"$(LOGDIR)\"" "PIDDIR=>\"$(PIDDIR)\""
	chmod +x log
	perl -c log; ../scripts/fixLocale.sh log

pwr_rowland: pwr.m4
	m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=rowland pwr.m4 > pwr_rowland

pwr_turbine: pwr.m4
	m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=turbine pwr.m4 > pwr_turbine

install_log: log config.xml pwr_rowland pwr_turbine
	@echo "Installing binaries for modbus"
	@$(CP) log $(BIN)/rowland_power_log
	@$(CP) log $(BIN)/turbine_power_log
	@echo "installing configuration"
	@$(CP) config.xml $(CONFIGDIR)/config.xml
	@echo "installing startup scripts"
	@$(CP) pwr_rowland /etc/init.d/
	@$(CP) pwr_turbine /etc/init.d/
clean:
	rm -f *.swp *~ *.log *.csv log test pwr_rowland pwr_turbine

