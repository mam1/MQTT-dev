#
# File:  Makefile (for MQTT library)
#

project_DIRECTORY := ~/Git/MQTT-Scheduler
program_INCLUDE_DIRS := $(project_DIRECTORY)/include
support_LIBRARY_DIRS := $(project_DIRECTORY)/lib

program_C_SRCS := $(wildcard ~/Git/MQTT-Scheduler/lib_sources/*.c)
program_CXX_SRCS := $(wildcard *.cpp)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)

# set directories
# ifeq ($(UNAME_S),Darwin)
program_INCLUDE_DIRS := $(program_INCLUDE_DIRS)
support_LIBRARY_DIRS := $(support_LIBRARY_DIRS)
support_LIBRARIES := MQTT.a

# set parameters
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(support_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(support_LIBRARIES),-l$(library))
CFLAGS  += -Wall
CXXFLAGS += $(CFLAGS)
LDFLAGS += $(CFLAGS) -fno-exceptions -fno-rtti 

# set locations for basic gnu tools
PREFIX := /media/sysuts/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC  := $(PREFIX)gcc
CXX := $(PREFIX)g++
LD  := $(PREFIX)ld
AS  := $(PREFIX)as
AR  := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy

# set libarary
LIB=libMQTT.a
LIBDEST= lib
LIBSRC := $(wildcard ~/Git/MQTT-Scheduler/lib_sources/*.c)
LIBOBJ=$(LIBSRC:.c=.o)

install: $(LIB)
	@echo lib Makefile - LIBOBJ $(LIBOBJ)
	@echo lib Makefile - libsrc $(LIBSRC)
	@echo lib Makefile - installing  $(LIB)
	@echo lib Makefile - into $(LIBDEST)
	@install -m 444 $(LIB) $(LIBDEST)

$(LIB): $(LIBOBJ)
	@echo lib Makefile - archiving $(LIB)
	@$(AR) r $(LIB) $(LIBOBJ)

.c.o:
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	@echo lib Makefile - cleaning up
	@- $(RM) -v $(LIBOBJ)

test:

	@echo $(HOLD)
	cd /home/mam1/Git/MQTT-Scheduler/lib_sources
	@echo $(ls -l)
	@echo sources = $(wildcard *.c)
	@echo program_C_SRCS $(program_C_SRCS)
	@echo LIBSRC $(LIBSRC)



