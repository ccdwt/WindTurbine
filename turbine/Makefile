UNAME := $(shell uname)
include ../make.inc

CCFLAGS := $(CCFLAGS) -D DEBUG
ifeq ($(UNAME), CYGWIN_NT-5.1)
	USBLIB = -llibusb-1.0
	EXT = .exe
else
	USBLIB 	= -lusb
endif

all: turbine logger asoc at

install: install_log install_test 

log: logger

test: turbine

util: asoc at

at : AT.o serial.o io.o XB.o 
	gcc $(CCFLAGS) -o at AT.o serial.o XB.o io.o SIP.o

asoc : asoc.o serial.o io.o XB.o SIP.o
	gcc $(CCFLAGS) -o asoc asoc.o serial.o XB.o io.o SIP.o

turbine : turbine.o serial.o io.o XB.o SIP.o
	gcc $(CCFLAGS) -o turbine turbine.o serial.o XB.o io.o SIP.o

logger : logger.o serial.o io.o XB.o SIP.o
	gcc $(CCFLAGS) -o logger logger.o serial.o XB.o io.o SIP.o

AT.o : AT.c io.h serial.h XB.h 
	gcc $(CCFLAGS) $(SERIAL_DEF) $(LOG_DEF) $(PID_DEF) $(CONFIG_DEF) -c AT.c

asoc.o : asoc.c SIP.h io.h serial.h XB.h
	gcc $(CCFLAGS) $(SERIAL_DEF) $(LOG_DEF) $(PID_DEF) $(CONFIG_DEF) -c asoc.c

turbine.o : turbine.c SIP.h io.h serial.h XB.h
	gcc $(CCFLAGS) $(SERIAL_DEF) $(LOG_DEF) $(PID_DEF) $(CONFIG_DEF) -c turbine.c

logger.o: logger.c SIP.h io.h serial.h XB.h
	gcc $(CCFLAGS) $(SERIAL_DEF) $(LOG_DEF) $(PID_DEF) $(CONFIG_DEF) -c logger.c

io.o : io.h serial.h io.c
	gcc $(CCFLAGS) $(SERIAL_DEF) -c io.c

serial.o : serial.h serial.c
	gcc $(CCFLAGS) -c serial.c

XB.o : serial.h XB.h io.h XB.c
	gcc $(CCFLAGS) -c XB.c

SIP.o : SIP.h SIP.c
	gcc $(CCFLAGS) -c SIP.c

init.sh: init.m4  
	m4 $(BIN_DEF) $(PID_DEF) $(LOG_DEF) init.m4 > init.sh	

################ INSTALL #######################


install_log: log install_startup
	@echo "Installing binary"
	@$(CP) logger $(BIN)/turbine_log

install_test: test
	$(CP) turbine $(BIN)/turbine_test

install_startup: init.sh
	@echo installing startup script
	@$(CP) init.sh /etc/init.d/turbine
	@chmod +x /etc/init.d/turbine

################## CLEAN ########################
clean: 
	rm -f *~ *.o turbine$(EXT) logger$(EXT) init.sh at$(EXT) asoc$(EXT)
