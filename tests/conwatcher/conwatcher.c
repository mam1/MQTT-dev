#include <unistd.h>
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/tokenizer.h"


#include "/usr/include/mariadb/mysql.h"

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tokenTest"
#define USER        "test-sql"
#define PSWD        "test-sql"


WINDOW * mainwin;

char 			linebuff[_INPUT_BUFFER_SIZE];
char 			screenbuff[500];
char 			tbuff[500];

char 			*lb_ptr, *lb_in, *lb_out, *lb_end;

void disp(char *str)
{
	clear();
	// mvprintw(1, 0, "                                                     ");
	mvprintw(1, 0, str);
	mvprintw(3, 0, screenbuff);
	mvprintw(30, 0, "enter a command > ");
	// mvprintw(30, 17, "                                                   ");
	mvprintw(30, 17, linebuff);
	refresh();
	return;
}

int main(int argc, char *argv[])
{
	lb_in = linebuff;
	lb_out = linebuff;
	lb_insert = linebuff
	            lb_ptr = linebuff;
	lb_end = linebuff + _INPUT_BUFFER_SIZE;

	char 		*ptr, *end_toke;
	int		ch;
	// static char keych[2] = {0};

	/*  Initialize ncurses  */
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(-1);
	}

	noecho();
	// cbreak();
	intrflush(mainwin, FALSE);
	keypad(mainwin, TRUE);
	// timeout(0);
	// nocbreak();
	clear();
	disp("conwatcher active");
	memset(linebuff, '\0', sizeof(linebuff));



	while ((ch = getch()) != 'q')  // read the keyboard
	{
		if ( isprint(ch) && !(ch & KEY_CODE_YES)) /*  If a printable character  */
		{
			if (lb_ptr <= lb_end - 1)		// room to add character ?
			{
				if (lb_ptr == lb_insert)
				{
					*lb_ptr++ = ch;
					lb_insert++;
					*lb_ptr = '\0';
				}
				else
				{
					end_toke = linebuff;
					while (*end_toke != '\0') end_toke++;
					*end_toke++ = '\0';
					while (end_toke > lb_insert)
						*end_toke = *--end_toke;
					*lb_ptr = ch;
				}
			}
			else
				disp("**** line buffer overflow ****");
			disp("got a character ");
		}
		else

			// { KEY_UP,        "Up arrow"        },
			// { KEY_DOWN,      "Down arrow"      },
			// { KEY_LEFT,      "Left arrow"      },
			// { KEY_RIGHT,     "Right arrow"     },
			// { KEY_HOME,      "Home"            },
			// { KEY_END,       "End"             },
			// { KEY_BACKSPACE, "Backspace"       },
			// { KEY_IC,        "Insert"          },
			// { KEY_DC,        "Delete"          },
			// { KEY_NPAGE,     "Page down"       },
			// { KEY_PPAGE,     "Page up"         },



			switch (ch)
			{

			case _NO_CHAR: 	/* NOCR */
				break;

			case  0x103:  	/* up arrow */


				break;
			case 0x102:		/* down arrow */


				break;

			case 0x105:		/* right arrow */


				break;
			case 0x104:		/* left arrow */

				break;

			case 0x107:		/* backspace */

				if (lb_ptr > linebuff )
				{
					*--lb_ptr = '\0';
					disp("got a backspace");
				}

				break;

		/* CR */	case 0xa:
				memset(screenbuff, '\0', sizeof(screenbuff));
				strcpy(screenbuff, linebuff);
				tokenizer(linebuff);
				memset(linebuff, '\0', sizeof(linebuff));
				disp ("linebuffer set to tokenizer to be processed");
				lb_in = linebuff;
				lb_out = linebuff;
				lb_ptr = linebuff;
				lb_end = linebuff + _INPUT_BUFFER_SIZE;
				break;

		/* DEL */	case 0x14a:
				disp("deleting token queue");
				memset(screenbuff, '\0', sizeof(screenbuff));
				memset(tbuff, '\0', sizeof(tbuff));
				while (Tpop(tbuff) != NULL)
				{
					strcat(screenbuff, tbuff);
					memset(tbuff, '\0', sizeof(tbuff));
					strcat(screenbuff, "\n");

				}
				disp ("token queue deleted");
				memset(linebuff, '\0', sizeof(linebuff));
				lb_in = linebuff;
				lb_out = linebuff;
				lb_ptr = linebuff;
				lb_end = linebuff + _INPUT_BUFFER_SIZE;

				break;
			}

	}

	delwin(mainwin);
	endwin();			/* End curses mode		  */
	refresh();
	printf("%s\n", "program terminated\n");
	return 0;
}