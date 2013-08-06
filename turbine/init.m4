#!/bin/bash
# start power logging

BIN=BINDIR
LOG=LOGDIR
PID=PIDDIR

EXEC="turbine_log"

function mypids(){
	MYPID=$(/usr/bin/pgrep $EXEC)
}

function clean(){
for pid in $(/usr/bin/pgrep $EXEC); do
	/bin/kill -9 $pid
done
}

if [ "$1" == "start" ]; then
	clean
	/bin/cp $LOG/turbine_error.log $LOG/turbine_error.log.1
	$BIN/$EXEC $LOG/$EXEC.csv 
elif [ "$1" == "stop" ]; then
#	/bin/kill $(cat $PID/$EXEC.pid)
	clean
fi

