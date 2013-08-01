PWD := $(shell pwd| sed -e 's!/\(db\|modbus\|turbine\|weather\|utilities\)!!')

CCFLAGS := -g -std=gnu99
BIN := $(PWD)/bin
CONFIGDIR := $(PWD)/bin
LOGDIR := $(PWD)/logs
PIDDIR := $(PWD)/pids
DBDIR := $(PWD)/db
WEATHER_INTERVAL := 20
WEATHER_HOST := weather
SERIAL := /dev/ttyO1

CONFIG_DEF := -D CONFIGDIR=\"$(CONFIGDIR)\"
LOG_DEF := -D LOGDIR=\"$(LOGDIR)\" 
PID_DEF := -D PIDDIR=\"$(PIDDIR)\" 
BIN_DEF := -D BINDIR=\"$(BIN)\" 
WEATHER_INTERVAL_DEF := -D WEATHER_INTERVAL=$(WEATHER_INTERVAL)
WEATHER_HOST_DEF := -D WEATHER_HOST=\"$(WEATHER_HOST)\"
SERIAL_DEF := -D SERIAL=\"$(SERIAL)\"

#utilities

CP=cp -u
SYM=ln -f --backup=none -s
SCRIPTS = $(PWD)/scripts
