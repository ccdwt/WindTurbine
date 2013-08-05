#!/bin/sh
##### adds configuration variables to perl files
IN="$1"
OUT="$2"

#### find the last use in the file
line="$(grep -n import $IN | tail -n 1)"
LINES="$(wc -l $IN | awk '{ print $1 }')"
LINENUM="${line%%:*}"

head -n $LINENUM $IN > $OUT;
### add new constants

echo "###################### CONSTANTS #################" >> $OUT
echo "" >> $OUT

for DEF in ${@:3}; do 
	echo "$DEF" >> $OUT
done

echo "" >> $OUT
echo "##################### END CONSTANTS ##############" >> $OUT
echo "" >> $OUT

## add rest of file;

tail -n $(expr $LINES - $LINENUM ) $IN >> $OUT

