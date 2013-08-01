include ../make.inc

all: log test

install: install_log install_test

test: test.pl config.xml
	perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml
	../scripts/perl_constants.sh log.pl log "CONFIGDIR=>\"$(CONFIGDIR)\"" "LOGDIR=>\"$(LOGDIR)\"" "PIDDIR=>\"$(PIDDIR)\""
	chmod +x log
	perl -c log; ../scripts/fixLocale.sh log

install_log: log config.xml
	$(CP) log $(BIN)/rowland_power_log
	$(CP) log $(BIN)/turbine_power_log
	$(CP) config.xml $(CONFIGDIR)/config.xml
	$(CP) pwr_rowland /etc/init.d/
	$(CP) pwr_turbine /etc/init.d/
clean:
	rm -f *.swp *~ *.log *.csv log test 

