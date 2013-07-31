include ../make.inc

all: log test

install: install_log install_test

test: test.pl config.xml
	perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml
	perl -c log.pl; ../scripts/fixLocale.sh log.pl
	../scripts/perl_constants.sh log.pl log "CONFIGDIR=>$(CONFIGDIR)" "LOGDIR=>$(LOGDIR)" "PIDDIR=>$(PIDDIR)"

install_log: log config.xml
	$(CP) log $(BIN)/rowland_power_log
	$(CP) log $(BIN)/turbine_power_log
	$(CP) config.xml $(CONFIGDIR)/config.xml

clean:
	rm -f *.swp *~ *.log  

