#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"


 /* buffer for semaphores */
 typedef struct sembuf SEMBUF;

/* system configuration data */
typedef struct {
	int         major_version;
	int         minor_version;
	int 		channels;
	int 		sensors;
	int 		commands;
	int 		states;
} _CONFIG_DAT;

/* structure of system file record */
typedef struct {
	_CONFIG_DAT 			config;					// system configuration
	int 					escape;
	char 					*line_q;			
} _SYS_DAT;

/* ipc data - memory mapped */
typedef	struct {
	int				force_update;					// force daemon to update relays
	_SYS_DAT		sys_data;						// persistant system data
	// _SEN_DAT		s_dat[_NUMBER_OF_SENSORS];  	// current sensor values - set by cgi
} _IPC_DAT;

typedef struct {
	char 		 			token[_TOKEN_BUFFER_SIZE];
	int 					value;
	char 					type[_TOKEN_BUFFER_SIZE];
		
} _TOKEN;


#endif
