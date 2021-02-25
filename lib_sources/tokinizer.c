
#include "/usr/include/mariadb/mysql.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/tokenizer.h"

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

/* command list */
static char    *keyword[_CMD_TOKENS] =
{
	/*  0 */    "temp",
	/*  1 */    "*",
	/*  2 */    "humid",
	/*  3 */    "schedule",
	/*  4 */    "?",
	/*  5 */    "display",
	/*  6 */    "yes",
	/*  7 */    "cancel",
	/*  8 */    "replace",
	/*  9 */    "sedit",
	/* 10 */    "delete",
	/* 11 */    "active",
	/* 12 */    "on",
	/* 13 */    "off",
	/* 14 */    "clear",
	/* 15 */    "status",
	/* 16 */    "time",
	/* 17 */    "sensor",
	/* 18 */    "cycle",
	/* 19 */    "ipc",
	/* 20 */    "show",
	/* 21 */    "save",
	/* 22 */    "template",
	/* 23 */    "hide",
	/* 24 */    "load",
	/* 25 */    "set",
	/* 26 */    "q",
	/* 27 */    "done",
	/* 28 */    "back",
	/* 29 */    "system",
	/* 30 */    "channel",
	/* 31 */    "ssch",
	/* 32 */    "wsch",
	/* 33 */    "slib",
	/* 34 */    "tlib",
	/* 35 */    "INT",
	/* 36 */    "STR",
	/* 37 */    "OTHER",
	/* 38 */	"slist",
	/* 39 */	"group",
	/* 40 */	"description"
};


static void show_mysql_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
	       mysql_sqlstate(mysql),
	       mysql_error(mysql));
	exit(-1);
}

/* return character type  */
// int char_type(char c) {
// 	switch (c) {
// 	case _COMMA:
// 	case _SPACE:
// 	case _COLON:
// 	case _SLASH:
// 		return "delimiter";
// 	case _QUOTE:
// 		return "quote";
// 	case _DEL:
// 		return "number";
// 	case _CR:
// 		return "enter";
// 	}
// 	return 4;
// }

/* return token type */
// char * (token_type)
// {


// }


/* test for a valid integer */
int is_valid_int(const char *str)
{
	if (*str == '-')     //negative numbers
		++str;
	if (!*str)           //empty string or just "-"
		return 0;
	while (*str)         //check for non-digit chars in the rest of the string
	{
		if (!isdigit(*str))
			return 0;
		else
			++str;
	}
	return -1;

}



/* return token type or command number */
char * token_type(char *c)
{
	int     i;
	char    *p;

	/*test for an empty command */
	if ((*c == '\0') || (*c == ' '))
		return "null";

	/* test for a integer */
	if (is_valid_int(c))
		return "integer";

	/* test for a keyword */
	for (i = 0; i < 35; i++)
	{
		if (strlen(c) == strlen(keyword[i])) {
			p = c;
			while (*p != '\0') {
				*p = tolower(*p);
				p++;
			};
			if (strncmp(c, keyword[i], strlen(c)) == 0)
				return "keyword";
		}
	}
	for (i = 38; i < _CMD_TOKENS ; i++)
	{
		if (strlen(c) == strlen(keyword[i])) {
			p = c;
			while (*p != '\0') {
				*p = tolower(*p);
				p++;
			};
			if (strncmp(c, keyword[i], strlen(c)) == 0)
				return "keyword 2";
		}
	}



	/* unrecognized token */
	return "unrecognized";
}


int is_a_delimiter(char * c)
{
	switch (*c)
	{
	case _COMMA:
		return 1;
		break;
	case _COLON:
		return 1;
		break;
	case _SLASH:
		return 1;
		break;
	case _SPACE:
		return 1;
		break;
	default:
		return 0;
		break;

	}

}

int Tpush(char * token, char * string)
{
	MYSQL               *conn;
	char 				buff[_INPUT_BUFFER_SIZE];
	if (*token == ' ') return 1;
	// printf("Tpush called with token <%s>, type <%s>\n", token, string);
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
	sprintf(buff, "INSERT INTO TokenQ(token, type) VALUES('%s','%s');", token, string);
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

	char 				buff[_INPUT_BUFFER_SIZE];
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
	// printf("we now have %i  active columns\n", mysql_num_fields(result));
	// for (i = 0; i < (int)mysql_num_fields(result); i++)
	// {
	// 	mysql_field_seek(result, i);
	// 	field = mysql_fetch_field(result);
	// 	printf("column %i  %s\n", i, field->name);
	// }
	// printf("\n");

	if ((row = mysql_fetch_row(result)) == NULL) return NULL;

	cptr = row[1];
	bptr = token;
	while (*cptr != '\0')
	{
		// printf("moving <%c>\n", *cptr);
		*bptr++ = *cptr++;
	}
	bptr = '\0';

	// printf("token is <%s>\n", row[1]);
	// printf("tokenID is <%s>\n", row[0]);

	sprintf(buff, "delete from TokenQ WHERE tokenID = '%s';", row[0]);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	return token;
}

int reset_tokenQ(void)
{
	char 			*tbuf_ptr, *lbuf_ptr;
	char 			tbuf[_INPUT_BUFFER_SIZE];

	memset (tbuf, '\0', _INPUT_BUFFER_SIZE);
	printf("%s\n", "crearing the token queue\n");
	while (Tpop(tbuf) != NULL)
	{
		printf("poped <%s>\n", tbuf);
		memset (tbuf, '\0', _INPUT_BUFFER_SIZE);
	}
	return 1;

}

int tokenizer(char *lbuf)
{

	char 			*tbuf_ptr, *lbuf_ptr;
	char 			tbuf[_INPUT_BUFFER_SIZE];

	tbuf_ptr = tbuf;
	lbuf_ptr = lbuf;

	memset(tbuf, '\0', _INPUT_BUFFER_SIZE);

	while((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++; // remove leading delimiters

	while (*lbuf_ptr != '\0')
	{	// loop until the input buffer is empty



		if (*lbuf_ptr == _QUOTE)  /* test for QUOTE */
		{
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			lbuf_ptr++;													// skip the quote
			while ((*lbuf_ptr != _QUOTE) && (*lbuf_ptr != '\0')) 		// look for ending quote or end of buffer
				*tbuf_ptr++ = *lbuf_ptr++;
			*(++tbuf_ptr) = '\0';
			lbuf_ptr++;
			lbuf_ptr++;
			Tpush(tbuf, "string");
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
		}


		if (is_a_delimiter(lbuf_ptr))
		{
			*tbuf_ptr = '\0';
			lbuf_ptr++;
			Tpush(tbuf, token_type(tbuf));
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++;
		}
		else
		{
			*tbuf_ptr++ = *lbuf_ptr++;
		}
	}
	*tbuf_ptr++ = *lbuf_ptr++;
	Tpush(tbuf, token_type(tbuf));
	// Tpush(tbuf, token_type(tbuf));
	return 0;
}
