#!/bin/bash
# start power logging

PID=PIDDIR
BIN=BINDIR
LOG=LOGDIR

EXEC="weather_log"
PIDFILE="$EXEC.pid"

function mypids(){
	MYPIDS=$(/usr/bin/pgrep $EXEC )
}

function clean(){
for PID in $(/usr/bin/pgrep $EXEC ); do
	/bin/kill $PID
done
}

if [ "$1" == "start" ]; then
	clean
	$BIN/$EXEC weather $LOG/$EXEC.csv
elif [ "$1" == "stop" ]; then
	#/bin/kill $(cat $PID/$PIDFILE)
	clean
fi
