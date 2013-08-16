PWD := $(shell pwd| sed -e 's!/\(db\|modbus\|turbine\|weather\|utilities/dropbox\|utilities/monit\)!!')

INSTALL_BASE := /usr/local
CCFLAGS := -g -std=gnu99
BIN := /usr/local/bin
CONFIGDIR := /usr/local/etc
LOGDIR := /usr/local/logs
PIDDIR := /var/run
DBDIR := /usr/local/turbine_DB
DB_NAME := test.db
WEATHER_INTERVAL := 20
WEATHER_HOST := weather
TURBINE_INTERVAL:= 20
DB_APP_KEY := 'pg85fmq2slj3p7j'
DB_APP_SECRET := 'ex3whqzzabg6w75'
DB_TOKEN_FILE := DropBoxToken.txt
SERIAL := /dev/ttyO1

CONFIG_DEF := -D CONFIGDIR=\"$(CONFIGDIR)\"
LOG_DEF := -D LOGDIR=\"$(LOGDIR)\" 
PID_DEF := -D PIDDIR=\"$(PIDDIR)\" 
BIN_DEF := -D BINDIR=\"$(BIN)\" 
DB_DEF  := -D DBDIR=\"$(DBDIR)\"
WEATHER_INTERVAL_DEF := -D WEATHER_INTERVAL=$(WEATHER_INTERVAL)
WEATHER_HOST_DEF := -D WEATHER_HOST=\"$(WEATHER_HOST)\"
SERIAL_DEF := -D SERIAL=\"$(SERIAL)\"

#utilities

CP=cp -u
SYM=ln -f --backup=none -s
SCRIPTS = $(PWD)/scripts
