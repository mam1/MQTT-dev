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
CPPFLAGS += $(foreach includedir,$(project_INCLUDE_DIRS),-I$(includedir), -I/usr/include)
LDFLAGS += $(foreach librarydir,$(project_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(project_LIB),-l$(library))
LDFLAGS += -lncurses
CFLAGS  += -Wall
CXXFLAGS += $(CFLAGS)
LDFLAGS += $(CFLAGS) -fno-exceptions      

# set host dependent locations for basic gnu tools and libraries
HOST=$(shell hostname)
ifeq ($(HOST),MQTT-beaglebone)
    PREFIX := 
else
    PREFIX := /media/sysuts/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
endif

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
	export project_DIRECTORY
	@echo ${CC}
	@echo "\n\n>>>>> building $(program_NAME) "

# 	$(CC) $(program_OBJS) -Xlinker --verbose -o $(project_BINARIES)/$(program_NAME) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
	$(CC) $(program_OBJS)  -o $(project_BINARIES)/$(program_NAME) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) 

# -Xlinker --verbose
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
	export project_DIRECTORY
	cd  $(project_DIRECTORY)/lib && $(MAKE) clean
	cd  $(project_DIRECTORY)/lib && $(MAKE)

sysbld:
		export project_DIRECTORY
	cd  $(project_DIRECTORY)/lib && $(MAKE) clean
	cd  $(project_DIRECTORY)/lib && $(MAKE)
	cd  $(project_DIRECTORY)/tests/conwatcher && $(MAKE)
	cd  $(project_DIRECTORY)/tests/toker && $(MAKE)
	cd  $(project_DIRECTORY)/tests/cp_daemon && $(MAKE)

dump:
	@echo hostname $(HOST)
	@echo GNU PREFIX $(PREFIX)
	@echo project_DIRECTORY $(project_DIRECTORY)
	@echo project_INCLUDE_DIRS $(project_INCLUDE_DIRS)
	@echo project_BINARIES $(project_BINARIES)
	@echo project_LIB $(project_LIB)
	@echo project_LIBRARY_DIRS $(project_LIBRARY_DIRS)
	@echo program_DIRECTORY $(program_DIRECTORY)
	@echo program_NAME $(program_NAME)

	@echo project_DIRECTORY $(project_DIRECTORY)
	@echo include_DIRECTORY $(include_DIRECTORY)
	@echo support_LIB $(support_LIB)

	@echo program_DIRECTORY $(program_DIRECTORY)
	@echo program_NAME $(program_NAME)

	@echo program_C_SRCS $(program_C_SRCS)

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
