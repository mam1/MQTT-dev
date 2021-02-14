#include <unistd.h>
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"

	WINDOW * mainwin;											// character typed on keyboard


char 			linebuff[_INPUT_BUFFER_SIZE];
char 			*lb_ptr, *lb_in, *lb_out, *lb_end;

void disp(char *str)
{
	wmove(mainwin, 3, 0);
	deleteln();
	mvprintw(3, 0, str);
	mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
	mvprintw(3, 17, linebuff);
	refresh();

	return;


}

int main(int argc, char *argv[])
{

	lb_in = linebuff;
	lb_out = linebuff;
	lb_ptr = linebuff;
	lb_end = linebuff + _INPUT_BUFFER_SIZE;
	int		ch;
	static char keych[2] = {0};



	/*  Initialize ncurses  */
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(-1);
	}

	/*  Switch of echoing and enable keypad (for arrow keys)  */
	noecho();
	cbreak();
	keypad(mainwin, TRUE);
	clear();
	mvprintw(0, 0, "conwatcher active");/* Move to (y, x) then print string     */
	mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
	refresh();
	memset(linebuff, '\0', sizeof(linebuff));
	while ((ch = getch()) != 'q')
	{
		// read the keyboard
		if ( isprint(ch) && !(ch & KEY_CODE_YES))
		{

			/*  If a printable character  */
			if (lb_ptr <= lb_end)		// room to add character ?
			{
				*lb_ptr++ = ch;
			}
			*lb_ptr = '\0';

			disp("got a character ");
		}
		else
		{

			switch (ch)
			{

		/* ESC */	case _ESC:
				disp("got a ESC")

				break;

		/* NOCR */	case _NO_CHAR:
				break;

		/* up arrow */	case 0x103:


				break;
		/* down arrow */case  0x102:


				break;

		/* right arrow */case  0x105:


				break;
		/* left arrow */case  0x104:


				break;

		/* CR */	case 0xa:
				disp("got a CR");
				memset(linebuff, '\0', sizeof(linebuff));
				lb_in = linebuff;
				lb_out = linebuff;
				lb_ptr = linebuff;
				lb_end = linebuff + _INPUT_BUFFER_SIZE;
				break;

		/* DEL */	case 0x14a:
				disp("got a DEL");

				break;

		/* OTHER */ default:
				// if (lb_ptr <= lb_end)		// room to add character ?
				// {
				// 	*lb_ptr++ = c;
				// }
				// 	wmove(mainwin, 3, 0);
				// 	deleteln();
				// 	mvprintw(30, 0, "enter a command > "); Move to (y, x) then print string
				// 	mvprintw(3, 17, linebuff);
				// 	refresh();
				// }

			}
		}
		/* do suff while waiting or the keyboard */


		/************************************************************************************************/
		endwin();			/* End curses mode		  */
		printf("linebuffer <%s>\n", linebuff);
		printf("%s\n", "program terminated\n");
		return 0;
	}

