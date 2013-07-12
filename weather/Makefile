all: test log

log: log.c weather.h
	gcc -o log log.c

test: test.c weather.h
	gcc -o test test.c

clean: 
	rm -rf test test.exe *.o *~ *.csv 
