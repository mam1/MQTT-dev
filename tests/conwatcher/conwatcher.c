#include <unistd.h>
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"



int main(int argc, char *argv[])
{
	char 			linebuff[_INPUT_BUFFER_SIZE];
	char 			*lb_in, *lb_out, *lb_ptr, *lb_end;
	uint8_t 		c;
	WINDOW * mainwin, * childwin;											// character typed on keyboard


	printf("%s\n", "conwatcher v-0.0 active");
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

    clear

	while (1)
	{
		c = getch();						// read the keyboard
		printf("\rswitching on <%c>\n", c);

		switch (c)
		{
		// /* NOCR */	case _NO_CHAR:
		// 		break;

		// /* ESC */ 	case _ESC:
		// 		c = getc(stdin);		// skip to next character
		// 		c = getc(stdin);		// skip to next character
		// 		switch (c)
		// 		{
		// 	/* up arrow */	case 'A':
		// 			if (rb_out_idx > 0)
		// 				rb_out_idx--;
		// 			else
		// 				rb_out_idx = rb_in_idx - 1;
		// 			if (rb_out_idx >= rb_in_idx)
		// 				rb_out_idx = 0;
		// 			arrow_reprompt();
		// 			continue;
		// 			break;
		// 	/* down arrow */case 'B':
		// 			rb_out_idx++;
		// 			if (rb_out_idx >= rb_in_idx)
		// 				rb_out_idx = 0;
		// 			arrow_reprompt();
		// 			continue;
		// 			break;

		// 	/* right arrow */case 'C':
		// 			if (input_ptr < work_buffer_ptr) {
		// 				input_ptr++;
		// 				printf("\033[1C");	// move cursor right
		// 			}
		// 			continue;
		// 			break;
		// 	/* left arrow */case 'D':
		// 			if (input_ptr > start_buff) {
		// 				input_ptr--;
		// 				printf("\033[1D");	// move cursor left
		// 			}
		// 			continue;
		// 			break;
		// 	/* ESC */		default:
		// 			escape = true;

		// 			continue;
		// 			break;
		// }
	/* CR */	case _CR:

			// 		// if (work_buffer_ptr != start_buff) 						// skip null input lines
			// // 		{
			// // 			if (strcmp(work_buffer, previous_work_buffer) != 0)	// remove duplicates
			// // 			{
			// // 				strcpy(&ring_buffer[rb_in_idx++][0], work_buffer);
			// // 				if (rb_in_idx > _CMD_BUFFER_DEPTH - 1)
			// // 					rb_in_idx = 0;
			// // 				rb_out_idx = rb_in_idx;
			// // 				memset(previous_work_buffer, '\0', sizeof(work_buffer));
			// // 				strcpy(previous_work_buffer, work_buffer);
			// // 			}
			// // 		}
			// // 		printf("\n\r");						// move cursor to next line
			// // 		*work_buffer_ptr++ = _CR;			// load the CR into the work buffer
			// // 		*work_buffer_ptr++ = '\0';			// load the NULL into the work buffer
			// // 		work_buffer_ptr = work_buffer;		// reset pointer
			// // 		char_fsm_reset();					// reset char_fsm
			// // 		while (*work_buffer_ptr != '\0')	// send characters to char_fsm
			// // 		{
			// // 			char_fsm(char_type(*work_buffer_ptr), &char_state, work_buffer_ptr);
			// // 			work_buffer_ptr++;
			// // 		}

			// // 		work_buffer_ptr = work_buffer;		// reset pointer
			// // 		input_ptr = work_buffer_ptr;		// reset pointer
			// // 		memset(work_buffer, '\0', sizeof(work_buffer));
			// // 		memset(screen_buf, '\0', sizeof(screen_buf));
			// // 		memset(&ring_buffer[rb_in_idx][0], '\0', _INPUT_BUFFER_SIZE);
			printf("got a CR\n");
			return 0;
			break;
			// /* DEL */	case _DEL:
			// 		if (input_ptr == start_buff)
			// 			break;

			// 		if (input_ptr == work_buffer_ptr) {	// no arrow keys in play
			// 			*work_buffer_ptr-- = '\0';
			// 			*work_buffer_ptr = '\0';
			// 			input_ptr = work_buffer_ptr;
			// 			printf("\r");
			// 			del_prompt(cmd_fsm_cb.state);		// display user prompt
			// 			printf("%s", work_buffer);		// print work_buffer
			// 			printf("\033[K");				// Erase to end of line
			// 			prompted = true;
			// 		}
			// 		else
			// 		{
			// 			mv = work_buffer_ptr - input_ptr;
			// 			input_ptr--;
			// 			hptr = input_ptr;
			// 			while (input_ptr < work_buffer_ptr)	// shift buffer left
			// 			{
			// 				*input_ptr = *(input_ptr + 1);
			// 				input_ptr++;
			// 			}
			// 			input_ptr = hptr;
			// 			*work_buffer_ptr-- = '\0';
			// 			*work_buffer_ptr = '\0';

			// 			printf("\r");
			// 			printf("\033[K");	// Erase to end of line
			// 			del_prompt(cmd_fsm_cb.state);
			// 			printf("%s", work_buffer);
			// 			while (mv > 0) {
			// 				printf("\033[1D");	// move cursor left
			// 				mv--;
			// 			}
			// 		}

			break;

	/* OTHER */ default:
			if (lb_ptr <= lb_end)		// room to add character ?
			{
				*lb_ptr++ = c;
				// printf("\033[1D");	// move cursor left
			}







			// {
			// 	if (input_ptr == work_buffer_ptr) 	// cursor is at the end of the input buffer
			// 	{
			// 		*work_buffer_ptr++ = c;
			// 		input_ptr = work_buffer_ptr;
			// 		printf("%c", c);
			// 	}
			// 	else 								// cursor is not at the end of the input buffer
			// 	{
			// 		move_ptr = work_buffer_ptr++;
			// 		move_ptr++;
			// 		*move_ptr-- = '\0';
			// 		while (move_ptr > input_ptr)
			// 		{
			// 			*move_ptr = *(move_ptr - 1);
			// 			move_ptr--;
			// 		}
			// 		*input_ptr++ = c;
			// 		mv = work_buffer_ptr - input_ptr;
			// 		printf("\r");
			// 		printf("\033[K");	// Erase to end of line
			// 		prompt(cmd_fsm_cb.state);
			// 		printf("%s", work_buffer);

			// 		while (mv > 0)
			// 		{
			// 			printf("\033[1D");	// move cursor left
			// 			mv--;
			// 		}
			// 	}
			// }
		}
		/* do suff while waiting or the keyboard */
	}

	/************************************************************************************************/
	endwin();			/* End curses mode		  */
	printf("linebuffer <%s>\n", linebuff);
	return 0;
}


/**************************************/

#