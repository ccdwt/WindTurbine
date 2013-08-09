include ../make.inc

all: log test

install: install_log install_test

test: test.pl config.xml deps
	@echo "checking sytax of test.pl"
	@perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml deps
	@echo "adding configuration to log"
	@$(SCRIPTS)/perl_constants.sh log.pl log "CONFIGDIR=>\"$(CONFIGDIR)\"" "LOGDIR=>\"$(LOGDIR)\"" "PIDDIR=>\"$(PIDDIR)\""
	@chmod +x log
	@echo "checking syntax of log"
	@perl -c log
	@$(SCRIPTS)/fixLocale.sh log

pwr_rowland: pwr.m4
	@m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=rowland pwr.m4 > pwr_rowland

pwr_turbine: pwr.m4
	@m4 $(BIN_DEF) $(LOG_DEF) $(PID_DEF) -D NAME=turbine pwr.m4 > pwr_turbine

install_log: log config.xml pwr_rowland pwr_turbine
	@echo "Installing binaries for modbus"
	@$(CP) log $(BIN)/rowland_power_log
	@$(CP) log $(BIN)/turbine_power_log
	@echo "installing configuration"
	@$(CP) config.xml $(CONFIGDIR)/modbus_power.xml
	@echo "installing startup scripts"
	@$(CP) pwr_rowland /etc/init.d/
	chmod +x /etc/init.d/pwr_rowland
	@$(CP) pwr_turbine /etc/init.d/
	chmod +x /etc/init.d/pwr_turbine

deps.pl:
	@echo generating dependency file for perl
	cat log.pl |grep ^use > deps.pl
deps: deps.pl
	@ echo -n "checking dependencies for modbus .. "
	@ if perl -c deps.pl; then echo "Present"; else \
		echo "Not present";\
		echo "Configuring CPAN";\
		perl -MCPAN -e "install CPAN";\
		echo "installing XML::Simple";\
		perl -MCPAN -e "install XML::Simple";\
		echo "installing Proc::Daemon";\
		perl -MCPAN -e "install Proc::Daemon";\
		fi

clean:
	rm -f *.swp *~ *.log *.csv log test pwr_rowland pwr_turbine

