#!/bin/bash
# start power logging

BIN=BINDIR
PID=PIDDIR
LOG=LOGDIR
PS="/bin/ps"
GREP="/bin/grep"
AWK="/usr/bin/awk"
EXEC="$BIN/`'NAME`'_power_log"

function clean(){
for PID in $($PS -ef |$GREP `'NAME`'_power_log | $GREP -v grep |$AWK '{print $2}'); do
	echo $PID
	kill $PID
done
}

if [ "$1" == "start" ]; then
	clean
	$EXEC &> $LOG/power_`'NAME`'_error.log &
elif [ "$1" == "stop" ]; then
	clean
fi
