#!/bin/bash
if [ -e "/usr/bin/dos2unix" ]; then
	CMD="/usr/bin/dos2unix $1"
 elif [ -e /bin/sed ]; then
 	CMD="/bin/sed -i -e 's/\r$//' $1"
elif [ -e /usr/bin/tr ]; then
	CMD="/usr/bin/tr -d '\r'< $1 > tmp; chmod --reference=$1 tmp; mv tmp $1"
else 
	echo "ERROR: you do not appear to have either dos2unix, sed, or tr. Cannot continue"
	exit 1
fi
echo "cmd = $CMD"
if [ -e $1 ]; then
	bash -c "$CMD"
else
	echo "$0: $1: No such file or directory"
	exit 1
fi
