#!/bin/bash
######## send a warning not to install unless production invironment #####

echo "This collection of programs was only meant to be installed on a BeagleBone Black, 
and will attempt to install some drivers that are specific to that platform. 
If you just want to install the components, try: 
	
	'$ make install_noarch' or 
	'$ make install_loggers install_db' 

Please refer to Readme.md for more information"
echo

read -e -p "Do you wish to continue?(Y/N)[N] " CHOICE

test "$CHOICE" == "y" -o "$CHOICE" == "Y"

exit $?
