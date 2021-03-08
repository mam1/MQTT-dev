#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include "/home/mam1/Git/MQTT-dev/include/shared.h"


/* buffer for semaphores */
typedef struct sembuf _SEMBUF;

typedef struct sembuf {
 unsigned short int sem_num; // semaphore #: 0 = first
 short int sem_op; // semaphore operation
 short int sem_flg; // operation flags
} _SEMBUF;

// /* system configuration data */
// typedef struct {
// 	int         major_version;
// 	int         minor_version;
// 	int 		channels;
// 	int 		sensors;
// 	int 		commands;
// 	int 		states;
// } _CONFIG_DAT;

/* structure of system file record */
// typedef struct {
// 	// _CONFIG_DAT 			config;					// system configuration
// 	// int 					escape;
// 	// char 					*line_q;
// 	char 				char 			linebuff[_INPUT_BUFFER_SIZE];
// } _SYS_DAT;

/* ipc data - memory mapped */
typedef	struct {
	char 					linebuff[_INPUT_BUFFER_SIZE];
} _IPC_DAT;

/* command token data */
typedef struct {
	char 		 			token[_TOKEN_BUFFER_SIZE];
	int 					value;
	char 					type[_TOKEN_BUFFER_SIZE];

} _TOKEN;


#endif
