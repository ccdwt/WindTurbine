UNAME := $(shell uname)
ifeq ($(UNAME), CYGWIN_NT-5.1)
	USBLIB = -llibusb-1.0
	EXT = .exe
else
	USBLIB 	= -lusb
endif

all: turbine logger asoc at

log: logger

test: turbine

util: asoc at

at : AT.o serial.o io.o XB.o 
	gcc -o at AT.o serial.o XB.o io.o SIP.o

asoc : asoc.o serial.o io.o XB.o SIP.o
	gcc -o asoc asoc.o serial.o XB.o io.o SIP.o

turbine : turbine.o serial.o io.o XB.o SIP.o
	gcc -o turbine turbine.o serial.o XB.o io.o SIP.o

logger : logger.o serial.o io.o XB.o SIP.o
	gcc -o logger logger.o serial.o XB.o io.o SIP.o

AT.o : AT.c io.h serial.h XB.h 
	gcc -c AT.c

asoc.o : asoc.c SIP.h io.h serial.h XB.h
	gcc -c asoc.c

turbine.o : turbine.c SIP.h io.h serial.h XB.h
	gcc -c turbine.c

logger.o: logger.c SIP.h io.h serial.h XB.h
	gcc -c logger.c

io.o : io.h serial.h io.c
	gcc -c io.c

myusb.o : myusb.h myusb.c
	gcc -c myusb.c

serial.o : serial.h serial.c
	gcc -c serial.c

XB.o : serial.h XB.h io.h XB.c
	gcc -c XB.c

SIP.o : SIP.h SIP.c
	gcc -c SIP.c

#usb_turbine.o : usb_turbine.c SIP.h myusb.h
#	gcc -c usb_turbine.c

clean: 
	rm -f *~ *.o turbine$(EXT) usb_turbine$(EXT) logger$(EXT)
