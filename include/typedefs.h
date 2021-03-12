#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include "/home/mam1/Git/MQTT-dev/include/shared.h"


/* buffer for semaphores */
// typedef struct {
// 	unsigned short int sem_num; // semaphore #: 0 = first
// 	short int sem_op; // semaphore operation
// 	short int sem_flg; // operation flags
// } _SEMBUF;

/* buffer for semaphores */
typedef struct sembuf SEMBUF;

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
// 	char 				char 			linebuff[_LINE_BUFFER_SIZE];
// } _SYS_DAT;

/* ipc data - memory mapped */
typedef	struct {
	int 			minor_version_cp_daemon;
	int 			major_version_cp_daemon;
	int 			minor_version_conwwatcher;
	int 			major_version_conwwatcher;
	int 			minor_version_toker;
	int 			major_version_toker;
	char 			screen_r[_SCREEN_R];
	char 			screen_c[_SCREEN_C];
	char 			screen_toker[_SCREEN_S];
	char 			linebuff[_LINE_BUFFER_SIZE];
} _IPC_DAT;

/* command token data */
typedef struct {
	char 		 			token[_TOKEN_BUFFER_SIZE];
	int 					value;
	char 					ttype[_TOKEN_BUFFER_SIZE];

} _TOKEN;


#endif
