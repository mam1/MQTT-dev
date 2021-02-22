
#include "/usr/include/mariadb/mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>
#include <time.h>

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

static void show_mysql_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
	       mysql_sqlstate(mysql),
	       mysql_error(mysql));
	exit(-1);
}

/* return character type  */
int char_type(char c) {
	switch (c) {
	case _COMMA:
	case _SPACE:
	case _COLON:
	case _SLASH:
		return "delimiter";
	case _QUOTE:
		return "quote";
	case _DEL:
		return "number";
	case _CR:
		return "enter";
	}
	return 4;
}

/* return token type */
char * (token_type)
{


}

int isadelimiter(char * c)
{
	switch (c) {
	case _COMMA:
	case _SPACE:
	case _COLON:
	case _SLASH:
		return 1;
	default:
		return -1;

	}

}

int Tpush(char * token, char * string)
{
	MYSQL               *conn;
	char 				buff[500];

	/* get connection handle  */
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

	/* insert newest token */
	sprintf(buff, "INSERT INTO TokenQ(token) VALUES('%s');", token, string);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);
	return 1;
}

char * Tpop(char * token)


int tokenizer(char *lbuf) {

	char 			*tbuf_ptr, *lbuf_ptr;
	char 			tbuf[500];
	char 			quote = 0;

	tbuf_ptr = tbuf;
	lbuf_ptr = lbuf;

	memset(tbuf, '\0', 500);

	while (*lbuf_ptr != '\0')
	{	// loop until the input buffer is empty

		/* NULL */
		if (lbuf_ptr == lbuf)
		{
			cobtinue
		}

		/* QUOTE */
		if (*lbuf_ptr == _QUOTE)
		{
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			lbuf_ptr++;													// skip the quote
			while ((*lbuf_ptr != _QUOTE) && (*lbuf_ptr != '\0')) 		// look for ending quote or end of buffer
				*tbuf_ptr++ = *lbuf_ptr++;								// move char to temp buffer
			*(++tbuf_ptr) = '\0';
			Tpush(tbuf, "string");
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
		}

		if (isadelimiter(*lbuf_ptr))
		{
			tbuf_ptr++ = '\0';
			Tpush(tbuf, token_type(tbuf));
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			// while ((!isadelimiter(*lbuf_ptr)) && (*lbuf_ptr != '\0')) 		// look for delimiter or end of buffer
			// 	*tbuf_ptr++ = *lbuf_ptr++;
		}

		*tbuf_ptr++ = *lbuf_ptr++;
	}
	*tbuf_ptr++ = *lbuf_ptr++;
	Tpush(tbuf, token_type(tbuf))
}
