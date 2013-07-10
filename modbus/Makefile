all: log test

test: test.pl config.xml
	perl -c test.pl

log: log.pl config.xml
	perl -c log.pl

clean:
	rm -f *.swp *~ *.log  

