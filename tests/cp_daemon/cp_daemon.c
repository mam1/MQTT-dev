/********************************************************************/
/*	Dcon.c - daemon updates the channel relays once a minute unless */
/*	the user requests an immediate update							*/
/********************************************************************/

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <ctype.h> 		//isalnum, tolower
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
// #include "Pcon.h"
// #include "Dcon.h"
// #include "ipc.h"
// #include "bitlit.h"
// #include "PCF8563.h"
// #include "sch.h"
// #include "trace.h"
// #include "typedefs.h"
// #include "sys_dat.h"
// #include "bbb.h"

// #include "/usr/include/mariadb/mysql.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
// #include "/home/mam1/Git/MQTT-dev/include/tokenizer.h"

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tokenTest"
#define USER        "test-sql"
#define PSWD        "test-sql"


/********** globals *******************************************************************/
// _IPC_DAT       	ipc_dat, *ipc_ptr;              // ipc data
// char           	ipc_file[] = {_IPC_FILE_NAME};  // name of ipc file
// void           	*data;                      	// pointer to ipc data
// int            	fd;                        		// file descriptor for ipc data file
// // int         	rtc;							// file descriptor for PCF8563 RTC
// _tm         	tm;								// time date structure
// key_t 			skey = _SEM_KEY;
// int 			semid;

// /* global memory mapped io variables */
// unsigned short 	semval;
// union semun {
// 	int val;              						// used for SETVAL only
// 	struct semid_ds *buf; 						// for IPC_STAT and IPC_SET
// 	ushort *array;        						// used for GETALL and SETALL
// };
// union 			semun dummy;
// SEMBUF sb = {0, -1, 0};  						// set to allocate resource 


/********** support functions *******************************************************************/

/* write an entry to the daemon log file */
void logit(char *mess){
	FILE 				*dlog;
	char 				* time_now;
	time_t 				t;

	t=time(NULL);
	time_now = ctime(&t);

	/* Open log file */
	dlog = fopen(_DAEMON_LOG, "a");
	if (dlog == NULL) {
		exit(EXIT_FAILURE);
	}

	fprintf(dlog,"%s - %s\n", time_now, mess);
	fclose(dlog);
	return;
}


int main(void) {
	
	pid_t 		pid, sid;		// process ID and Session ID
	int 		toggle;
	int 		h_min;
	// _tm 		t;
	int 		ipc;
	FILE 		*pidf;
	char 		command[120];
	int 		i;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then
	   we can exit the parent process. */
	if (pid > 0) {
		/* create pid file */
		pidf = fopen(_PID_FILE_NAME, "w");
		if (pidf != NULL){
			
			fprintf(pidf,"%i", pid);
			fclose(pidf);
		}
		else{
			printf(" can't write pid file <%s>\n", _PID_FILE_NAME);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* Daemon-specific initializations */
	sprintf(command, "\n ************************************\n daemon %i.%i.%i started\n",0, 0, 0);
	logit(command);
	logit("starting initializations"); 

	/* The Big Loop */
	logit("initialization complete");
	logit("starting main loop");
	while (1) 
	{

		logit("looping");
		usleep(3000000);
	}
	exit(EXIT_SUCCESS);
}