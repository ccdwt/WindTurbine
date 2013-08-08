#!/bin/sh
### BEGIN INIT INFO
# Provides:		monit
# Required-Start: 	$syslog $networking
# Required-Stop:  	$syslog
# Default-Start:	2 3 4 5
# Default-Stop:		1
# Short-Description:	monit monitoring software
### END INIT INFO
MONIT="/usr/local/bin/monit"
start(){
	echo -n Starting monit
	$MONIT start all
	$MONIT
	echo
}

stop(){
	echo -n Stopping monit: 
	$MONIT quit
	echo
}

reload(){
	echo -n Reloading monit: 
	$MONIT reload
	echo
}

case "$1" in 
	start)
		start
		;;
	stop)
		stop
		;;
	restart|condrestart)
		stop
		start
		;;
	reload)
		reload
		;;
	*)
		echo $"Usage: $0 {start|stop|restart|reload|status}"
		exit 1
		;;
esac

exit 0
