#include <unistd.h>
#include <stdio.h>
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"



int main(int argc, char *argv[])
{
	/* semaphores */
	key_t           	skey = _SEM_KEY;
	int             	semid;

	/* global memory mapped io variables */
	unsigned short    semval;
	union semun {
		int val;                            // used for SETVAL only
		struct semid_ds *buf;               // for IPC_STAT and IPC_SET
		ushort *array;                      // used for GETALL and SETALL
	};
	union       semun dummy;
	SEMBUF sb = {0, -1, 0};               // set to allocate resource

	int 			cmd_buffer_push_index, cmd_buffer_pop_index;
	char 			cmd_buffer[_CMD_BUFFER_DEPTH][_INPUT_BUFFER_SIZE + 1]; // array to hold multiple single arrays of characters

	uint8_t 		c;											// character typed on keyboard
	int 			prompted = false;							// has a prompt been sent
	int 			fd;											// file descriptor for ipc data file
	char 			work_buffer[_INPUT_BUFFER_SIZE];			// contains the user input
	char 			*work_buffer_ptr, *end_buff, *start_buff, *move_ptr;
	char 			previous_work_buffer[_INPUT_BUFFER_SIZE];	// points to the  previous user input

	char 			screen_buf[_SCREEN_BUFFER_SIZE], *cursor_ptr;
	char 			*input_ptr, *hptr;

	char 			ring_buffer[_CMD_BUFFER_DEPTH][_INPUT_BUFFER_SIZE];	// char array[NUMBER_STRINGS][STRING_MAX_SIZE];
	int 			rb_in_idx, rb_out_idx;
	int 			mv;
	int  			escape;				// have we just processed an escape


	/* set up file mapped shared memory for inter process communication */
	ipc_sem_init();										// setup semaphores
	semid = ipc_sem_id(skey);							// set semaphor id

	/* set up shared memory */
	ipc_sem_lock(semid, &sb);							// wait for a lock on shared memory
	fd = ipc_open(ipc_file, ipc_size());      			// create/open ipc file
	data = ipc_map(fd, ipc_size());           			// map file to memory
	ipc_ptr = data; 									// overlay data with _IPC_DAT data structure
	ipc_sem_free(semid, &sb);


	printf("%s\n", "conwatcher active");

	/**********************************************************************************************/

	while (1)
	{
		c = getc(stdin);						// read the keyboard
		switch (c)
		{
	/* NOCR */	case _NO_CHAR:
			break;

	/* ESC */ 	case _ESC:
			c = getc(stdin);		// skip to next character
			c = getc(stdin);		// skip to next character
			switch (c)
			{
		/* up arrow */	case 'A':
				if (rb_out_idx > 0)
					rb_out_idx--;
				else
					rb_out_idx = rb_in_idx - 1;
				if (rb_out_idx >= rb_in_idx)
					rb_out_idx = 0;
				arrow_reprompt();
				continue;
				break;
		/* down arrow */case 'B':
				rb_out_idx++;
				if (rb_out_idx >= rb_in_idx)
					rb_out_idx = 0;
				arrow_reprompt();
				continue;
				break;

		/* right arrow */case 'C':
				if (input_ptr < work_buffer_ptr) {
					input_ptr++;
					printf("\033[1C");	// move cursor right
				}
				continue;
				break;
		/* left arrow */case 'D':
				if (input_ptr > start_buff) {
					input_ptr--;
					printf("\033[1D");	// move cursor left
				}
				continue;
				break;
		/* ESC */		default:
				escape = true;


				while (pop_cmd_q(cmd_fsm_cb.token)); 						// empty command queue
				memset(work_buffer, '\0', sizeof(work_buffer));				// clean out work buffer
				memset(previous_work_buffer, '\0', sizeof(work_buffer));	// clean out previous command buffer
				work_buffer_ptr = work_buffer;								// set pointer to start of buffer
				input_ptr = work_buffer;									// set pointer to start of buffer
				char_fsm_reset();											// initialize the character fsm
				cmd_fsm_reset(&cmd_fsm_cb); 								// initialize the command processor fsm
				char_state = 0;
				prompted = false;											// force a prompt
				printf("\n\rcommand processor reset\n\r");
				strcpy(cmd_fsm_cb.prompt_buffer, "enter a command");
				continue;
				break;
			}
			// /* CR */	case _CR:

			// 		if (work_buffer_ptr != start_buff) 						// skip null input lines
			// 		{
			// 			if (strcmp(work_buffer, previous_work_buffer) != 0)	// remove duplicates
			// 			{
			// 				strcpy(&ring_buffer[rb_in_idx++][0], work_buffer);
			// 				if (rb_in_idx > _CMD_BUFFER_DEPTH - 1)
			// 					rb_in_idx = 0;
			// 				rb_out_idx = rb_in_idx;
			// 				memset(previous_work_buffer, '\0', sizeof(work_buffer));
			// 				strcpy(previous_work_buffer, work_buffer);
			// 			}
			// 		}
			// 		printf("\n\r");						// move cursor to next line
			// 		*work_buffer_ptr++ = _CR;			// load the CR into the work buffer
			// 		*work_buffer_ptr++ = '\0';			// load the NULL into the work buffer
			// 		work_buffer_ptr = work_buffer;		// reset pointer
			// 		char_fsm_reset();					// reset char_fsm
			// 		while (*work_buffer_ptr != '\0')	// send characters to char_fsm
			// 		{
			// 			char_fsm(char_type(*work_buffer_ptr), &char_state, work_buffer_ptr);
			// 			work_buffer_ptr++;
			// 		}

			// 		work_buffer_ptr = work_buffer;		// reset pointer
			// 		input_ptr = work_buffer_ptr;		// reset pointer
			// 		memset(work_buffer, '\0', sizeof(work_buffer));
			// 		memset(screen_buf, '\0', sizeof(screen_buf));
			// 		memset(&ring_buffer[rb_in_idx][0], '\0', _INPUT_BUFFER_SIZE);

			// 		break;
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
			// 		break;

			// /* OTHER */ default:
			// 		if (work_buffer_ptr <= end_buff)		// room to add character ?
			// 		{
			// 			if (input_ptr == work_buffer_ptr) 	// cursor is at the end of the input buffer
			// 			{
			// 				*work_buffer_ptr++ = c;
			// 				input_ptr = work_buffer_ptr;
			// 				printf("%c", c);
			// 			}
			// 			else 								// cursor is not at the end of the input buffer
			// 			{
			// 				move_ptr = work_buffer_ptr++;
			// 				move_ptr++;
			// 				*move_ptr-- = '\0';
			// 				while (move_ptr > input_ptr)
			// 				{
			// 					*move_ptr = *(move_ptr - 1);
			// 					move_ptr--;
			// 				}
			// 				*input_ptr++ = c;
			// 				mv = work_buffer_ptr - input_ptr;
			// 				printf("\r");
			// 				printf("\033[K");	// Erase to end of line
			// 				prompt(cmd_fsm_cb.state);
			// 				printf("%s", work_buffer);

			// 				while (mv > 0)
			// 				{
			// 					printf("\033[1D");	// move cursor left
			// 					mv--;
			// 				}
			// 			}
			// 		}
		}
		/* do suff while waiting or the keyboard */
	}

	/************************************************************************************************/

	return 0;
}