#!/bin/bash


MY_HOSTNAME=MQTT-beaglebone
TEST_HOSTNAME=$(hostname)

echo -e "\nMY_HOSTNAME " $MY_HOSTNAME
echo -e "TEST_HOSTNAME " $TEST_HOSTNAME

if [[ $(hostname) = 'MQTT-beaglebone' ]]
 	then echo -e "It's a bone\n"
 	else echo -e "It's not a bone.......\n"	
fi

# set locations for basic gnu tools
if [[ $(hostname) =~ 'MQTT-beaglebone' ]]
 	then 
 		echo "It's a bone . . . \n"
		PREFIX=""
 	else 
 		echo "It's not a bone\n"	
		PREFIX=/media/sysuts/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
fi

echo -e "PREFIX = " $(PREFIX)
	CC =$(PREFIX)gcc
	CXX=$(PREFIX)g++
	LD =$(PREFIX)ld
	AS =$(PREFIX)as
	AR =$(PREFIX)ar
	OBJCOPY=$(PREFIX)objcopy


echo -e "CC = " $(CC)

