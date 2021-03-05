
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


static void show_mysql_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
	       mysql_sqlstate(mysql),
	       mysql_error(mysql));
	exit(-1);
}

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
_TOKEN * token_type(_TOKEN *token)
{
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_ROW           row;
	char 				buff[_INPUT_BUFFER_SIZE];

	/*test for an empty command */
	if ((*token->token == '\0') || (*token->token == ' '))
	{
		strcpy(token->type, "null");
		return token;
	}

	/* test for a integer */
	if (is_valid_int(token->token))
	{
		strcpy(token->type, "integer");
		token->value = (int) strtol(token->token, (char **)NULL, 10);
		return token;
	}

	/* test for a key word */

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

	sprintf(buff, "SELECT * FROM KeyWords WHERE keyword = '%s';", &(token->token[0]));
	if (mysql_query(conn, buff))
		show_mysql_error(conn);
	result = mysql_store_result(conn);

	if ((row = mysql_fetch_row(result)) == NULL) {
		strcpy(token->type, "unrecognized");
		token->value = -1;
		return token;
	}
	strcpy(token->type, "keyword");
	token->value = (int) strtol(row[3], (char **)NULL, 10);
	return token;
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

int Tpush(char *token_buffer)
{
	MYSQL               *conn;
	char 				buff[_INPUT_BUFFER_SIZE];
	_TOKEN 				token;

	char 				*tptr, *bptr;
	tptr = token.token;
	bptr = token_buffer;

	/*test for an empty command */
	if ((*token_buffer == '\0') || (*token_buffer == ' '))
		return 1;

	memset(&token.token, '\0', sizeof(token.token));
	while (*bptr != '\0') *tptr++ = *bptr++;

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

	token_type(&token);			//set token type and value 
	/* insert newest token */
	sprintf(buff, "INSERT INTO TokenQ(token, type, value) VALUES('%s','%s', '%i');", token.token, token.type, token.value);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);
	return 1;
}

int Qpush(char * token_buffer)
{
	MYSQL               *conn;
	char 				buff[_INPUT_BUFFER_SIZE];

	if (*token_buffer == ' ') return 1;

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

	/* insert newest token_buffer */
	sprintf(buff, "INSERT INTO TokenQ(token, type, value) VALUES('%s','%s', NULL);", token_buffer, "string");
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);
	return 1;
}

_TOKEN * Tpop(_TOKEN *token)
{
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_ROW           row;
	char 				*cptr, *bptr;
	char 				buff[_INPUT_BUFFER_SIZE];

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

	if ((row = mysql_fetch_row(result)) == NULL)
	 return NULL;

	cptr = row[1];
	bptr = token->token;
	while (*cptr != '\0')
	{
		*bptr++ = *cptr++;
	}
	bptr = '\0';

	sprintf(buff, "delete from TokenQ WHERE tokenID = '%s';", row[0]);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	return token;
}
int reset_tokenQ(void)
{

	_TOKEN 			tbuf;

	memset (&tbuf, '\0', sizeof(tbuf));

	while ((char *)Tpop(&tbuf) != NULL)
	{

		memset (&tbuf, '\0', sizeof(tbuf));
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

	while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++; // remove leading delimiters
	while (*lbuf_ptr != '\0')  // loop until the line buffer is empty
	{
		if (*lbuf_ptr == _QUOTE)  /* test for QUOTE */
		{
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			lbuf_ptr++;													// skip the quote
			while ((*lbuf_ptr != _QUOTE) && (*lbuf_ptr != '\0')) 		// look for ending quote or end of buffer
				*tbuf_ptr++ = *lbuf_ptr++;
			*(tbuf_ptr) = '\0';
			lbuf_ptr++;
			lbuf_ptr++;
			Qpush(tbuf);
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
		}

		if (is_a_delimiter(lbuf_ptr))
		{
			*tbuf_ptr = '\0';
			lbuf_ptr++;
			Tpush(tbuf);
			memset(tbuf, '\0', sizeof(tbuf));
			tbuf_ptr = tbuf;
			while ((is_a_delimiter(lbuf_ptr)) && (*lbuf_ptr != '\0')) lbuf_ptr++;
		}
		else
			*tbuf_ptr++ = *lbuf_ptr++;
	}
	*tbuf_ptr++ = *lbuf_ptr++;
	Tpush(tbuf);
	return 0;
}
