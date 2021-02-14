#include <unistd.h>
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curses.h>
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"


int main(int argc, char *argv[])
{
	char 			linebuff[_INPUT_BUFFER_SIZE];
	char 			*lb_ptr, *lb_in, *lb_out, *lb_end;

	int		ch;
	static char keych[2] = {0};

	WINDOW * mainwin;											// character typed on keyboard

	lb_in = linebuff;
	lb_out = linebuff;
	lb_ptr = linebuff;
	lb_end = linebuff + _INPUT_BUFFER_SIZE;


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
			wmove(mainwin, 3, 0);
			deleteln();

			mvprintw(3, 0, "got a character ", (char)(ch));

			mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
			mvprintw(3, 17, linebuff);
			refresh();
		}
		else
		{

			switch (ch)
			{

		/* ESC */	case _ESC:
				wmove(mainwin, 3, 0);
				deleteln();
				mvprintw(3, 0, "got a ESC\n");
				mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
				refresh();
				continue;
				break;

		/* NOCR */	case _NO_CHAR:
				break;

		/* up arrow */	case 0x103:

				continue;
				break;
		/* down arrow */case  0x102:

				continue;
				break;

		/* right arrow */case  0x105:

				continue;
				break;
		/* left arrow */case  0x104:

				continue;
				break;

		/* CR */	case 0xa:
				wmove(mainwin, 3, 0);
				deleteln();
				mvprintw(3, 0, "got a _CR");
				// mvprintw(30, 0, "enter a command > ");   // Move to (y, x) then print string     
				refresh();
				return 0;
				break;

		/* DEL */	case 0x14a:
				wmove(mainwin, 3, 0);
				deleteln();
				mvprintw(3, 0, "got a DEL");
				mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
				refresh();

				break;

		/* OTHER */ default:
				// if (lb_ptr <= lb_end)		// room to add character ?
				// {
				// 	*lb_ptr++ = c;
				// }
				wmove(mainwin, 3, 0);
				deleteln();
				mvprintw(3, 0, "got a problem");
				mvprintw(30, 0, "enter a command > ");/* Move to (y, x) then print string     */
				refresh();
			}

		}
	}
	/* do suff while waiting or the keyboard */


	/************************************************************************************************/
	endwin();			/* End curses mode		  */
	printf("linebuffer <%s>\n", linebuff);
	printf("%s\n", "program terminated\n");
	return 0;
}

