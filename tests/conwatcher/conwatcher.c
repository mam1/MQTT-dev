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

#include "/usr/include/mariadb/mysql.h"

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tokenTest"
#define USER        "test-sql"
#define PSWD        "test-sql"


WINDOW * mainwin;

char 			linebuff[_INPUT_BUFFER_SIZE];
char 			*lb_ptr, *lb_in, *lb_out, *lb_end;

void disp(char *str)
{
	char 			*ptr;

	ptr = str;

	// while(*ptr != '\n') ptr++;
	// *ptr = '\0';
	
	mvprintw(3,0,"                                                     ");
	mvprintw(3, 0, str);
	mvprintw(30, 0, "enter a command > ");
	mvprintw(30, 17, linebuff);
	refresh();
	return;
}

static void show_mysql_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
	       mysql_sqlstate(mysql),
	       mysql_error(mysql));
	exit(-1);
}

int Tpush(char * token)
{
	MYSQL               *conn;
	char 				buff[500];

	/* get connection handle  */
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("\ncouldn't initialize conn: %s\n", mysql_error(conn));
		exit(1);
	}

	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("\ncouldn't connect to database\n");
		exit(1);
	}

	/* insert newest token */
	sprintf(buff, "INSERT INTO TokenQ(token) VALUES('%s');", token);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);
	return 1;
}

char * Tpop(char * token)
{
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_FIELD         *field;
	MYSQL_ROW           row;

	char 				*cptr, *bptr;

	char 				buff[500];
	int 				i;

	/* get handles  */
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("couldn't initialize conn: %s\n", mysql_error(conn));
		exit(1);
	}
	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("couldn't connect to database\n");
		exit(1);
	}


	/*get the oldest row */
	if (mysql_query(conn, "SELECT * FROM TokenQ ORDER BY tokenID LIMIT 1;"))
		show_mysql_error(conn);

	result = mysql_store_result(conn);
	printf("we now have %i  active columns\n", mysql_num_fields(result));
	for (i = 0; i < (int)mysql_num_fields(result); i++)
	{
		mysql_field_seek(result, i);
		field = mysql_fetch_field(result);
		printf("column %i  %s\n", i, field->name);
	}
	printf("\n");

	if ((row = mysql_fetch_row(result)) == NULL) return NULL;

	cptr = row[1];
	bptr = token;
	while (*cptr != '\0')
	{
		printf("moving <%c>\n", *cptr);
		*bptr++ = *cptr++;
	}
	bptr = '\0';

	printf("token is <%s>\n", row[1]);
	printf("tokenID is <%s>\n", row[0]);

	sprintf(buff, "delete from TokenQ WHERE tokenID = '%s';", row[0]);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	return token;
}
int tokenizer(char *lbuf) {

	char 			*tbuf_ptr, *lbuf_ptr;
	char 			tbuf[500];

	tbuf_ptr = tbuf;
	lbuf_ptr = lbuf;

	memset(tbuf, '\0', 500);

	while (*lbuf_ptr != '\0') {
		while (*lbuf_ptr == ' ') lbuf_ptr++;
		while ((*lbuf_ptr != ' ') & (*lbuf_ptr != '\0')) *tbuf_ptr++ = *lbuf_ptr++;
		*tbuf_ptr = '\0';


		Tpush(tbuf);
		memset(tbuf, '\0', 500);
		tbuf_ptr = tbuf;

	}
	return 1;
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
	disp("conwatcher active");

	memset(linebuff, '\0', sizeof(linebuff));

	while ((ch = getch()) != _ESC)
	{
		// read the keyboard
		if ( isprint(ch) && !(ch & KEY_CODE_YES))
		{
			/*  If a printable character  */
			if (lb_ptr <= lb_end - 1)		// room to add character ?
			{
				*lb_ptr++ = ch;
				*lb_ptr = '\0';
			}
			else
				disp("**** line buffer overflow ****");
			disp("got a character ");
		}
		else
		{

			switch (ch)
			{

		/* ESC */	case _ESC:
				disp("got a ESC");
				return 0;
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

				tokenizer(linebuff);
				memset(linebuff, '\0', sizeof(linebuff));
				lb_in = linebuff;
				lb_out = linebuff;
				lb_ptr = linebuff;
				lb_end = linebuff + _INPUT_BUFFER_SIZE;
				disp ("linebuffer set to tokenizer to be processed");
				break;

		/* DEL */	case 0x14a:
				disp("got a DEL");

				break;
			}
		}

	}
	endwin();			/* End curses mode		  */
	printf("linebuffer <%s>\n", linebuff);
	printf("%s\n", "program terminated\n");
	return 0;
}