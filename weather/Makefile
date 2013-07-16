include ../make.inc

all: test log

log: log.c weather.h
	gcc $(CCFLAGS) -o log log.c

test: test.c weather.h
	gcc $(CCFLAGS) -o test test.c

clean: 
	rm -rf test test.exe *.o *~ *.csv 
