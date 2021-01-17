#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_



typedef {int;} _offset;


typedef struct {
	int			 	base;
	_offset		 	head;
	_offset		 	tail;
} _list;

typedef struct {
	int				base;
	_offset		 	head;
	_offset		 	tail;
} _stack;

typedef struct {
	_offset		 	previous;
	_offset		 	next;
	-offset 	 	payload;
	int				payload_type
	int				payload_size;
} _node;

/* schedule record */
typedef struct {
	int			key;
	int 		state;
	float 		temp;
	float 		humid;

} _schedule_rec;				

/* schedule  */
typedef struct {
	char 			name[_TEMPLATE_NAME_SIZE];
	int 			rcnt;
	_schedule		rec[_MAX_SCHEDULE_RECS +1];




/* structure of system file record */
typedef struct {
	_CONFIG_DAT 	config;							// system configuration
	_S_TAB  		sys_sch;						// system schedule
	_CHN_DAT     	c_data[_NUMBER_OF_CHANNELS];	// persistent channel data
	_TMPL_DAT 		tpl_lib[_MAX_TEMPLATES];		// schedule template library
	_S_TAB 			sch_lib[_MAX_SCHEDULES]; 		// schedule table library
	int 			sch_index;						// points to the next schedule table library available
	int 			tpl_index;						// points to the next template library available
} _SYS_DAT;





/* ipc data - memory mapped */
typedef	struct {
	int				force_update;					// force daemon to update relays
	_SYS_DAT		sys_data;						// persistant system data
	_SEN_DAT		s_dat[_NUMBER_OF_SENSORS];  	// current sensor values - set by cgi
} _IPC_DAT;



#endif
