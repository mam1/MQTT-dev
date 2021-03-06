#ifndef _SHARE_H_
#define _SHARE_H_

/* version info */
#define _TAG		"v0.0"

/* file names */
#define _FILE_NAME_SIZE 		50
#define _IPC_FILE_NAME			"/home/mam1/ipc.dat"   			// memory mapped ipc file
#define _IPC_FILE_BACKUP_NAME	"/home/mam1/ipc.bkup"   			// ipc backup file 

/* log files */
#define _SYSTEM_LOG				"/home/mam1/logs/syslog.log"		// system log file name
#define _DAEMON_LOG 			"/home/mam1/logs/cp_daemon.log" 	// c_daemon log file name
#define _CONWATCHER_LOG 		"/home/mam1/logs/conwatcher.log" 	// conwatcher log file name
#define _TOKER_LOG 				"/home/mam1/logs/conwatcher.log" 	// toker log file name

#define _PID_FILE_NAME 				"/home/mam1/.cp_daemon.pid"		// daemon pid

/* semaphore key */
#define _SEM_KEY 				1234321

/* character parser fsm */
#define _CHAR_TOKENS     		5
#define _CHAR_STATES     		4
#define _MAX_TOKEN_SIZE			128
#define _TT_INT					-1
#define _TT_STR					-2
#define _TT_NULL				-3
#define _TT_UNREC				-4

/* conwatcher screens */
#define _LINES  				60
#define _COLUMNS				100
#define _RLINES					58
#define _SLINES					1
#define _CLINES					1
#define _SCREEN_R 				_COLUMNS * _RLINES
#define _SCREEN_S 				_COLUMNS * _SLINES
#define _SCREEN_C 				_COLUMNS * _CLINES

/* command parser fsm */
#define _CMD_TOKENS     		41
#define _CMD_STATES     		35 

/* key codes */
#define _ESC        27
#define _SPACE      32
#define _COLON      58
#define _SLASH      47
#define _COMMA      44
#define _BS         8
#define _DEL		127
#define _QUOTE      34
#define _CR         13
#define _NL 		10
#define _FF 		12
#define _EOF		0
#define _NO_CHAR    255
#define _DELIMITER	28	
#define _UPA		124

/* buffers */
#define _LINE_BUFFER_SIZE		500
#define _TOKEN_BUFFER_SIZE		100
#define _PROMPT_BUFFER_SIZE		500
#define _CMD_BUFFER_DEPTH 		10 
#define _TRACE_BUFFER_SIZE 		128		
#define _SCREEN_BUFFER_SIZE		_LINE_BUFFER_SIZE + _PROMPT_BUFFER_SIZE

/* sensor parameteres */
#define _NUMBER_OF_SENSORS		20
#define _GROUP_NAME_SIZE 		9
#define _DESCRIPTION_NAME_SIZE	_MAX_TOKEN_SIZE
#define _MAX_SENSOR_ID			99

/* channel parameters */
#define _NUMBER_OF_CHANNELS 	16
#define _CHANNEL_NAME_SIZE		20

/* schedule parameters */
#define _SCHEDULE_NAME_SIZE		40
#define _MAX_SCHEDULE_RECS		50
#define _MAX_SCHEDULES			24
#define _ALL_CHANNELS			99
#define _ALL_DAYS				99

/* template library parameters */
#define _MAX_TEMPLATES			10
#define _TEMPLATE_NAME_SIZE		40

// #define _SCHEDULE_SIZE			_MAX_SCHEDULE_RECS + 1
// #define _SCHEDULE_BUFFER		128

#define	_BYTES_PER_INT 			4
#define _BYTES_PER_SCHEDULE 	_SCHEDULE_SIZE * _BYTES_PER_INT
#define _BYTES_PER_DAY 			_NUMBER_OF_CHANNELS * _BYTES_PER_SCHEDULE 	

/*	gpio pin assignment {header,pin,gpio}  	*/
// #define _CHAN0			{8,17,27}
// #define _CHAN1			{8,16,46}
// #define _CHAN2			{8,26,61}
// #define _CHAN3			{8,18,65}
// #define _CHAN4			{9,15,48}
// #define _CHAN5			{9,12,60}
// #define _CHAN6			{9,41,20} 
// #define _CHAN7			{9,23,49}
// #define _CHAN8			{9,18,4} //
// #define _CHAN9			{8,36,80}
// #define _CHAN10			{8,37,78}
// #define _CHAN11			{8,38,79}
// #define _CHAN12			{8,39,76}
// #define _CHAN13			{8,40,77} 
// #define _CHAN14			{8,41,74} 
// #define _CHAN15			{8,42,75}
// #define _HB0 			{8,7,66}
// #define _HB1 			{8,9,67}
// #define _HB2 			{8,8,69}
// #define _HB3 			{8,10,68}


#define _CHAN0			{8,27,86}
#define _CHAN1			{8,28,88}
#define _CHAN2			{8,29,87}
#define _CHAN3			{8,30,89}
#define _CHAN4			{8,31,10}
#define _CHAN5			{8,32,11}
#define _CHAN6			{8,33,9}
#define _CHAN7			{8,34,81}
#define _CHAN8			{8,35,8}
#define _CHAN9			{8,36,80}
#define _CHAN10			{8,37,78}
#define _CHAN11			{8,38,79}
#define _CHAN12			{8,39,86}
#define _CHAN13			{8,40,77}
#define _CHAN14			{8,41,74}
#define _CHAN15			{8,42,75}
#define _HB0 			{8,7,66}
#define _HB1 			{8,9,67}
#define _HB2 			{8,8,69}
#define _HB3 			{8,10,68}


/* general */
#define _DAYS_PER_WEEK 	7
#define _ON 			1
#define _OFF 			0
#define _TRUE 			1
#define _FALSE			0

/* macros */
#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

#endif	//_SHARE_H_
