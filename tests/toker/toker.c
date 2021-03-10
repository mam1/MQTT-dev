#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <curses.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

/* local includes */
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/tokenizer.h"
#include "/home/mam1/Git/MQTT-dev/include/logs.h"
// #include "/home/mam1/Git/MQTT-dev/include/ipc.h"

/* database connection */
#include "/usr/include/mariadb/mysql.h"
#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tokenTest"
#define USER        "test-sql"
#define PSWD        "test-sql"

#define DAY 		2
#define HOUR 		4
#define MINUTE 		22

#define MINUTES_PER_HOUR 		60
#define MINUTES_PER_DAY 		1440

/********** globals *******************************************************************/
_IPC_DAT       	ipc_dat, *ipc_ptr;              // ipc data
char           	ipc_file[] = {_IPC_FILE_NAME};  // name of ipc file
void           	*data;                      	// pointer to ipc data
int            	fd;                        		// file descriptor for ipc data file
key_t 			skey = _SEM_KEY;
int 			semid;

/* global memory mapped io variables */
unsigned short 	semval;
union semun {
	int val;              						// used for SETVAL only
	struct semid_ds *buf; 						// for IPC_STAT and IPC_SET
	ushort *array;        						// used for GETALL and SETALL
};
union 			semun dummy;
SEMBUF sb = {0, -1, 0};  						// set to allocate resource



int main(void)
{
logit(_TOKER_LOG, "toker", "toker started");

	char lbuf[] = {"abcdegf"};
	char 			*tbuf_ptr, *lbuf_ptr;
	char 			tbuf[_INPUT_BUFFER_SIZE];
		int 					ipc;

		/* check for ipc file */
	if (access(ipc_file, F_OK) == 0) {
		ipc = 1;
		logit(_CONWATCHER_LOG, "conwatcher", "ipc file found");
	}
	else {
		ipc = 0;
		logit(_CONWATCHER_LOG, "conwatcher", "* ipc file not found");
	}

	/* set up file mapped shared memory for inter process communication */
	ipc_sem_init();										// setup semaphores
	semid = ipc_sem_id(skey);

	/* setup shared memory */
	ipc_sem_init();
	semid = ipc_sem_id(skey);					// get semaphore id
	ipc_sem_lock(semid, &sb);					// wait for a lock on shared memory
	fd = ipc_open(ipc_file, ipc_size());      	// create/open ipc file
	data = ipc_map(fd, ipc_size());           	// map file to memory
	ipc_ptr = (_IPC_DAT *)data;					// overlay ipc data structure on shared memory
	ipc_sem_free(semid, &sb);                   // free lock on shared memory

	// printf("hi from toker\n");
	// /* setup semaphores */
	// int id = semget(KEY, 1, 0666 | IPC_CREAT);
	// if (id < 0)
	// {
	// 	perror("semget"); exit(11);
	// }
	// union semun u;
	// u.val = 1;
	// if (semctl(id, 0, SETVAL, u) < 0)
	// {
	// 	perror("semctl"); exit(12);
	// }

	/* setup shared memory */
	// sb.sem_num = 0; // semaphore #: 0 = first
	// sb.sem_op = -1; // semaphore operation
	// sb.sem_flg = 0; // operation flags
	// ipc_sem_init();
	// semid = ipc_sem_id(skey);					// get semaphore id
	// ipc_sem_lock(semid, &sb);					// wait for a lock on shared memory
	// fd = ipc_open(ipc_file, ipc_size());      	// create/open ipc file
	// data = ipc_map(fd, ipc_size());           	// map file to memory
	// ipc_ptr = (_IPC_DAT *)data;					// overlay ipc data structure on shared memory
	// ipc_sem_free(semid, &sb);                   // free lock on shared memory


	tbuf_ptr = tbuf;
	lbuf_ptr = lbuf;
	memset(tbuf, '\0', _INPUT_BUFFER_SIZE);
	
	strcpy(lbuf, ipc_ptr->linebuff);
	while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++; // remove leading delimiters
	while (*lbuf_ptr != '\0')  // loop until the line buffer is empty
	{
		if (*lbuf_ptr == _QUOTE)  /* test for QUOTE */
		{
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			lbuf_ptr++;													// skip the quote
			while ((*lbuf_ptr != _QUOTE) && (*lbuf_ptr != '\0')) 		// look for ending quote or end of buffer
				*tbuf_ptr++ = *lbuf_ptr++;
			*(tbuf_ptr) = '\0';
			lbuf_ptr++;
			lbuf_ptr++;
			Qpush(tbuf);
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
		}

		if (is_a_delimiter(lbuf_ptr))
		{
			*tbuf_ptr = '\0';
			lbuf_ptr++;
			Tpush(tbuf);
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++;
		}
		else
			*tbuf_ptr++ = *lbuf_ptr++;
	}
	*tbuf_ptr++ = *lbuf_ptr++;
	Tpush(tbuf);
	logit(_TOKER_LOG, "toker", "toker terminated");
	return 0;
}
