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
char 			*lb_ptr, *lb_in, *lb_out, *lb_end, *lb_insert, *ripple_ptr;

void disp(char *str)
{
	clear();
	mvprintw(1, 0, str);
	mvprintw(3, 0, screenbuff);
	mvprintw(30, 0, "enter a command > ");
	mvprintw(30, 17, linebuff);
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
	{
		*push++ = *pull++;
	}

	return;
}

int main(int argc, char *argv[])
{
	reset_linebuffer();

	char 		*ptr, *end_toke;
	int		ch;
	int 			x, y;

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
				}
				else 								// insert curser has been moved
				{
					end_toke = linebuff;			// find the end of the entered data
					while (*end_toke != '\0') end_toke++;
					*++end_toke = '\0';				// make room for the added character

					ripple_up(lb_insert, end_toke);

					while (end_toke > lb_insert)
						*end_toke = *--end_toke;	// ripple up the linbuffer

					*lb_insert++ = ch;
				}
			}
			else
			{
				disp("**** line buffer overflow ****");
			}

			disp("got a character ");

		}

		else
		{
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
				if (lb_insert < lb_ptr )
				{
					lb_insert++;
					getyx(mainwin, y, x);
					move(y, x + 1);
				}

				break;
			case 0x104:		/* left arrow */
				if (lb_insert > linebuff )
				{
					lb_insert--;
					getyx(mainwin, y, x);
					move(y, x - 1);
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
					ripple_ptr = lb_insert;
					while (*(ripple_ptr + 1) != '\0')
						*ripple_ptr = *++ripple_ptr;
					*ripple_ptr = '\0';
					lb_ptr = ripple_ptr;


				}

				break;

		/* CR */	case 0xa:
				memset(screenbuff, '\0', sizeof(screenbuff));
				strcpy(screenbuff, linebuff);
				tokenizer(linebuff);
				disp ("linebuffer set to tokenizer to be processed");
				reset_linebuffer();
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
				reset_linebuffer();


				break;
			}

		}
	}
	delwin(mainwin);
	endwin();			/* End curses mode		  */
	refresh();
	printf("%s\n", "program terminated\n");
	return 0;
}