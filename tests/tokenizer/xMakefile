
# export project_LIBRARY_DIRS := $(project_DIRECTORY)/lib /media/sysuts/mariadb-connector-c-3.1.11-ubuntu-focal-amd64/lib/mariadb
# export project_INCLUDE_DIRS := $(project_DIRECTORY)/include /media/sysuts/mariadb-connector-c-3.1.11-debian-9-stretch-amd64/include/mariadb
# export project_LIBRARY_DIRS := /home/mam1/Git/MQTT-dev/lib /media/sysuts/mariadb-connector-c-3.1.11-debian-9-stretch-amd64/lib/mariadb

# set host dependent locations for basic gnu tools and libraries
HOST=$(shell hostname)
ifeq ($(HOST),MQTT-beaglebone)
    PREFIX := 
    project_INCLUDE_DIRS := $(project_DIRECTORY)/include /usr/include/mariadb
    project_LIBRARY_DIRS := /home/mam1/Git/MQTT-dev/lib /usr/bin/mariadb
else
    PREFIX := /media/sysuts/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
    project_INCLUDE_DIRS := $(project_DIRECTORY)/include /media/sysuts/mariadb-connector-c-3.1.11-debian-9-stretch-amd64/include/mariadb
    project_LIBRARY_DIRS := $(project_DIRECTORY)/lib /media/sysuts/mariadb-connector-c-3.1.11-ubuntu-focal-amd64/lib/mariadb
endif

#  export the environmental varabiles
export project_INCLUDE_DIRS
export project_LIBRARY_DIRS
export project_INCLUDE_DIRS 
export project_LIBRARY_DIRS 
export project_DIRECTORY=~/Git/MQTT-dev
export project_BINARIES := $(project_DIRECTORY)/bin
export project_LIB := MQTT mariadbclient


program_DIRECTORY := ~/Git/MQTT-dev/tests/tokenizer
program_NAME := tokenizer
include $(project_DIRECTORY)/mastermake/Makefile
