all: log test

test: test.pl config.xml
	perl -c test.pl; ../scripts/fixLocale.sh test.pl

log: log.pl config.xml
	perl -c log.pl; ../scripts/fixLocale.sh log.pl

clean:
	rm -f *.swp *~ *.log  

