#include <unistd.h>
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

/* database connection */
#include "/usr/include/mariadb/mysql.h"
#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tokenTest"
#define USER        "test-sql"
#define PSWD        "test-sql"

// #define KEY 0x1111

// union semun {
// 	int val;
// 	struct semid_ds *buf;
// 	unsigned short  *array;
// };

// struct sembuf p = { 0, -1, SEM_UNDO};
// struct sembuf v = { 0, +1, SEM_UNDO};

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


WINDOW * mainwin;

char 			linebuff[_INPUT_BUFFER_SIZE];

char 			screenbuff[500];
// char 			screenbuff[500];
// char 			screenbuff[500];

char 			tbuff[500];
char 			*lb_ptr, *lb_in, *lb_out, *lb_end, *lb_insert, *ripple_ptr;
int 			x, y;


void disp(char *str)
{
	clear();
	mvprintw(1, 0, str);
	mvprintw(3, 0, screenbuff);
	mvprintw(30, 0, "enter a command > ");
	mvprintw(30, 17, linebuff);
	if (lb_ptr != lb_insert)			// insert curser has not been moved
		move(y, x);
	refresh();
	return;
}

void reset_linebuffer(void)
{
	memset(linebuff, '\0', sizeof(linebuff));
	lb_insert = linebuff;
	lb_ptr = linebuff;
	lb_end = linebuff + _INPUT_BUFFER_SIZE;
	disp("linebuffer reset");
	return;
}

void ripple_up(char *lb_insert, char *end_toke)
{
	char 			*pull;
	char 			*push;

	push = end_toke;
	pull = end_toke;
	pull--;

	while (push > lb_insert)
		*push-- = *pull--;

	return;
}

void ripple_down(char *lb_insert, char *end_toke)
{
	char 			*pull;
	char 			*push;

	push = lb_insert;
	pull = lb_insert;
	pull++;

	while (pull < end_toke)
		*push++ = *pull++;
	*--pull = '\0';
	lb_insert--;
	lb_ptr--;

	return;
}

void add_comm(char *cmd)
{
	return;
}

int main(int argc, char *argv[])
{
	logit("/home/mam1/logs/conwatcher.log","conwatcher: ", "conwatcher started");
	return 0;

	char 					*end_toke;
	int						ch;
	_TOKEN 					toke;
	int 					tpid;
	// int 					ipc;
	// char 					*args[];

	/* setup semaphores */
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

	/*  Initialize ncurses  */
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(-1);
	}
	noecho();
	intrflush(mainwin, FALSE);
	keypad(mainwin, TRUE);
	clear();
	disp("conwatcher active");
	reset_linebuffer();

	/* read the keyboard */
	while ((ch = getch()) != 'q')
	{
		if ( isprint(ch) && !(ch & KEY_CODE_YES)) 	// if a printable character
		{
			if (lb_ptr <= lb_end - 1)				// room to add character ?
			{
				if (lb_ptr == lb_insert)			// insert curser has not been moved
				{
					*lb_ptr++ = ch;					// insert character into linbuffer
					lb_insert++;
					*lb_ptr = '\0';
					disp("got a printable character ");
				}

				else 								// insert curser has been moved
				{
					// getyx(mainwin, y, x);

					end_toke = linebuff;			// find the end of the entered data
					while (*end_toke != '\0') end_toke++;
					*++end_toke = '\0';				// make room for the added character


					ripple_up(lb_insert, end_toke);

					*lb_insert++ = ch;

					// wmove(mainwin, 10, 10);
					// refresh();

					disp("inserting a character");
					x++;
					wmove(mainwin, y, x);
				}
			}
			else
			{
				disp("**** line buffer overflow ****");
			}
		}

		else
		{
			switch (ch)
			{

			case _NO_CHAR: 	/* NOCR */
				break;

			case  0x103:  	/* up arrow */


				break;
			case 0x102:		/* down arrow */


				break;

			case 0x105:		/* right arrow */
				if (lb_insert < lb_ptr )
				{
					lb_insert++;
					getyx(mainwin, y, x);
					x++;
					move(y, x);
				}

				break;
			case 0x104:		/* left arrow */
				if (lb_insert > linebuff )
				{
					lb_insert--;
					getyx(mainwin, y, x);
					x--;
					move(y, x);
				}
				break;

			case 0x107:		/* backspace */
				if ((lb_insert == lb_ptr) & (lb_ptr > linebuff ))
				{
					*--lb_ptr = '\0';
					lb_insert--;
					disp("got a backspace");
				}

				else
				{
					end_toke = linebuff;			// find the end of the entered data
					while (*end_toke != '\0') end_toke++;
					if (lb_insert > linebuff)
					{
						lb_insert--;
						ripple_down(lb_insert, end_toke);
						disp("deleting a character");
						x--;
						wmove(mainwin, y, x);
					}
				}
				break;

			case 0xa:		/* CR */
				memset(screenbuff, '\0', sizeof(screenbuff));
				strcpy(screenbuff, linebuff);

				//*************************************************
				// if (semop(id, &p, 1) < 0)
				// {
				// 	perror("semop p"); exit(13);
				// }

				// copy linbuffer into shared memory


				// if (semop(id, &v, 1) < 0)
				// {
				// 	perror("semop p"); exit(14);
				// }
				disp(ipc_ptr->linebuff);
				sleep(300000);
				tpid = vfork();
				if (tpid == 0) execl(" / usr / bin / mybins / toker", " / usr / bin / mybins / toker", (char *) 0);

				if (tpid < 0)
				{
					/*fork creation faile*/
					printf("fork creation failed!!!\n");
					exit (1);
				}


				//****************************************



				// tokenizer(linebuff);
				disp ("linebuffer sent to tokenizer to be processed");
				add_comm(linebuff);
				reset_linebuffer();
				break;

		/* DEL */	case 0x14a:
				disp("deleting token queue");
				memset(screenbuff, '\0', sizeof(screenbuff));
				while (Tpop(&toke) != NULL)
				{
					strcat(screenbuff, " token <");
					strcat(screenbuff, toke.token);
					strcat(screenbuff, ">,  type <");
					strcat(screenbuff, toke.type);
					strcat(screenbuff, ">, value < ");
					char  b[10];
					sprintf(b, " % i", toke.value);
					strcat(screenbuff, b);
					strcat(screenbuff, " > \n");
%s
					memset(toke.token, '\0', sizeof(toke.token));
					memset(toke.type, '\0', sizeof(toke.token));
					toke.value = 0;

				}
				disp ("token queue deleted");
				reset_linebuffer();


				break;

			case 0x168: /* End */
				delwin(mainwin);
				endwin();			/* End curses mode		  */
				refresh();
				printf("\n%s\n", "program terminated");
				return 0;
				break;
			}

		}
	}
	delwin(mainwin);
	endwin();			/* End curses mode		  */
	refresh();
	printf("\n%s\n", "program terminated");
	return 0;
}