#!/bin/bash
# start power logging

BIN=BINDIR
PID=PIDDIR
LOG=LOGDIR

EXEC="$BIN/`'NAME`'_power_log"

function clean(){
for PID in $(/usr/bin/pgrep `'NAME`'_power); do
	kill $PID
done
}

if [ "$1" == "start" ]; then
	clean
	$EXEC &> $LOG/power_`'NAME`'_error.log &
elif [ "$1" == "stop" ]; then
	clean
fi
