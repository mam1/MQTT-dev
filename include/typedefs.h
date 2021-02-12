#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/* line queue data strcuctures  */
typedef int 		_offset;

typedef struct {
	int				base;
	_offset		 	head;
	_offset		 	tail;
} _fifo_stack;

typedef struct {
	_offset		 	previous;
	_offset		 	next;
	_offset 	 	payload;
	int				payload_type;
	int				payload_size;
} _node;

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
	_CONFIG_DAT 			config;							// system configuration
	int 					escape;
	char 					*line_q;			
} _SYS_DAT;

/* ipc data - memory mapped */
typedef	struct {
	int				force_update;					// force daemon to update relays
	_SYS_DAT		sys_data;						// persistant system data
	_SEN_DAT		s_dat[_NUMBER_OF_SENSORS];  	// current sensor values - set by cgi
} _IPC_DAT;


#endif
