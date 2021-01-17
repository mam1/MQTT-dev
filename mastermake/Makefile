#############################################################################
#	basic cross compile build
#	target - BeagleBone (Debian)														#
############################################################################# 

# scan working directory
program_C_SRCS := $(wildcard *.c)
program_CXX_SRCS :=  $(wildcard *.cpp)
program_C_OBJS :=  ${program_C_SRCS:.c=.o}
program_CXX_OBJS :=  ${program_CXX_SRCS:.cpp=.o}
program_OBJS :=  $(program_C_OBJS) $(program_CXX_OBJS)

# set parameters
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
CFLAGS  += -Wall
CXXFLAGS += $(CFLAGS)
LDFLAGS += $(CFLAGS) -fno-exceptions      

# set locations for basic gnu tools
PREFIX := /media/sysuts/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC  := $(PREFIX)gcc
CXX := $(PREFIX)g++
LD  := $(PREFIX)ld
AS  := $(PREFIX)as
AR  := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy

# set libarary
LIB=MQTT

.PHONY: all clean distclean

# create executable - link in project library
all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	@echo ${CC}
	@echo "\n>>>>> building $(program_NAME) "

# 	$(CC) $(program_OBJS) -o $(system_BINARIES)/$(program_NAME) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
	$(CC) $(program_OBJS) -o $(system_BINARIES)/$(program_NAME) $(CFLAGS) $(CPPFLAGS) $(support_LIB)


	@echo ">>>>> build of  $(program_NAME) completed"

# clean up
	@- $(RM) -v $(program_NAME)
	@- $(RM) -v $(program_OBJS)

clean:
	@- $(RM) -v $(program_NAME)
	@- $(RM) -v $(program_OBJS)
	cd .. && $(MAKE) clean

# rebuild the project ibrary from lib-sources 
lib: 
	cd  ~/Git/MQTT-Scheduler/lib && $(MAKE) clean
	cd  ~/Git/MQTT-Scheduler/lib && $(MAKE)

dump:


	@echo project_DIRECTORY := $(project_DIRECTORY)
	@echo include_DIRECTORY := $(include_DIRECTORY)
	@echo support_LIB := $(support_LIB)

	@echo working_DIRECTORY := $(working_DIRECTORY)
	@echo program_NAME := $(program_NAME)

	@echo program_C_SRCS := $(program_C_SRCS)

	@echo CC ............... $(CC)
	@echo cc ............... $(cc)
	@echo LINK ............. $(LINK)
	@echo LINK.cc .......... $(LINK.cc)
	@echo program_NAME ..... $(program_NAME)
	@echo project_LIB ...... $(project_LIB)
	@echo program_C_OBJS ... $(program_C_OBJS)
	@echo program_OBJS ..... $(program_OBJS)
	@echo LDFLAGS .......... $(LDFLAGS)
	@echo CPPFLAGS ......... $(CPPFLAGS)
	@echo CFLAGS ........... $(CFLAGS)

distclean: cleans
