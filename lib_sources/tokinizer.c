
#include "/usr/include/mariadb/mysql.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/tokenizer.h"
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"
#include "/home/mam1/Git/MQTT-dev/include/logs.h"

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


#define STRING_SIZE 100

#define SELECT_SAMPLE "SELECT tokenID, token, type, value FROM TokenQ ORDER BY tokenID LIMIT 1"
// #define SELECT_SAMPLE "SELECT tokenID, token, type, value FROM TokenQ WHERE !SLEEP(1)"


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
	char 				buff[_LINE_BUFFER_SIZE];

	/*test for an empty command */
	if ((*token->token == '\0') || (*token->token == ' '))
	{
		strcpy(token->ttype, "null");
		return token;
	}

	/* test for a integer */
	if (is_valid_int(token->token))
	{
		strcpy(token->ttype, "integer");
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
		strcpy(token->ttype, "unrecognized");
		token->value = -1;
		return token;
	}
	strcpy(token->ttype, "keyword");
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
	char 				buff[_LINE_BUFFER_SIZE];
	_TOKEN 				token;

	char 				*tptr, *bptr;
	tptr = token.token;
	bptr = token_buffer;

	logit(_TOKER_LOG, "Tpush", "started");


	/*test for an empty command */
	if ((*token_buffer == '\0') || (*token_buffer == ' '))
		return 1;

	memset(&token.token, '\0', sizeof(token.token));
	strcpy(token.token, token_buffer);
	token_type(&token);			//set token type and value

	logit(_TOKER_LOG, "Tpush", "token_type called");
	logit(_TOKER_LOG, "Tpush token", token.token);
	logit(_TOKER_LOG, "Tpush token type", token.ttype);


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
	sprintf(buff, "INSERT INTO TokenQ(token, ttype, value) VALUES('%s','%s', '%i');", token.token, token.ttype, token.value);
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);


	return 1;
}

int Qpush(char * token_buffer)
{
	MYSQL               *conn;
	char 				buff[_LINE_BUFFER_SIZE];

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
	sprintf(buff, "INSERT INTO TokenQ(token, ttype, value) VALUES('%s','%s', NULL);", token_buffer, "string");
	if (mysql_query(conn, buff))
		show_mysql_error(conn);

	mysql_close(conn);
	return 1;
}

_TOKEN * Tpop(_TOKEN * t)
{

	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[4];
	MYSQL_RES     *prepare_meta_result;
	MYSQL_TIME    ts;
	unsigned long length[4];
	int           param_count, column_count, row_count;
	short         small_data;
	int           tokenID, int_data_2;
	char          str_data_1[STRING_SIZE];
	char          str_data_2[STRING_SIZE];
	my_bool       is_null[4];
	my_bool       error[4];
	MYSQL *mysql = NULL;
	int j;
	my_bool reconnect = 1;

	char 				buff[_LINE_BUFFER_SIZE];

	if (mysql_library_init(0, NULL, NULL)) {
		fprintf(stderr, "could not initialize MySQL client library\n");
		exit(1);
	}

	mysql = mysql_init(mysql);

	if (!mysql) {
		puts("Init faild, out of memory?");
		return NULL;
	}

	mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);

	mysql_options(mysql, MYSQL_READ_DEFAULT_FILE, (void *)"./test.cnf");

	if (!mysql_real_connect(mysql,       /* MYSQL structure to use */
	                        NULL,         /* server hostname or IP address */
	                        NULL,         /* mysql user */
	                        NULL,          /* password */
	                        NULL,           /* default database to use, NULL for none */
	                        0,           /* port number, 0 for default */
	                        NULL,        /* socket file or named pipe name */
	                        CLIENT_FOUND_ROWS /* connection flags */ )) {
		puts("Connect failed\n");
		return NULL;
	}

	// printf("connection id: %ld\n", mysql_thread_id(mysql));

	/* Prepare a SELECT query to fetch data from test_table */
	stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}
	if (mysql_stmt_prepare(stmt, SELECT_SAMPLE, strlen(SELECT_SAMPLE)))
	{
		fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	// fprintf(stdout, " prepare, SELECT successful\n");

	/* Get the parameter count from the statement */
	param_count = mysql_stmt_param_count(stmt);
	// fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

	if (param_count != 0) /* validate parameter count */
	{
		fprintf(stderr, " invalid parameter count returned by MySQL\n");
		exit(0);
	}

	/* Fetch result set meta information */
	prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (!prepare_meta_result)
	{
		fprintf(stderr,
		        " mysql_stmt_result_metadata(), returned no meta information\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	/* Get total columns in the query int_data*/
	column_count = mysql_num_fields(prepare_meta_result);
	// fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

	if (column_count != 4) /* validate column count */
	{
		fprintf(stderr, " invalid column count returned by MySQL\n");
		exit(0);
	}

	/* Bind the result buffers for all 4 columns before fetching them */

	memset(bind, 0, sizeof(bind));

	/* INTEGER COLUMN */
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = (char *)&tokenID;
	bind[0].is_null = &is_null[0];
	bind[0].length = &length[0];
	bind[0].error = &error[0];

	/* STRING COLUMN */
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char *)t->token;
	bind[1].buffer_length = STRING_SIZE;
	bind[1].is_null = &is_null[1];
	bind[1].length = &length[1];
	bind[1].error = &error[1];

	/* STRING COLUMN */
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (char *)t->ttype;
	bind[2].buffer_length = STRING_SIZE;
	bind[2].is_null = &is_null[2];
	bind[2].length = &length[2];
	bind[2].error = &error[2];

	/* INTEGER COLUMN */
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = (char *)&t->value;
	bind[3].is_null = &is_null[3];
	bind[3].length = &length[3];
	bind[3].error = &error[3];

	/* Bind the result buffers */
	if (mysql_stmt_bind_result(stmt, bind))
	{
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	/* Execute the SELECT query */
	if (mysql_stmt_execute(stmt))
	{
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
	} else {

		/* Now buffer all results to client */
		if (mysql_stmt_store_result(stmt))
		{
			fprintf(stderr, " mysql_stmt_store_result() failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}

		/* Fetch all rows */
		row_count = 0;
		// fprintf(stdout, "Fetching results ...\n");
		while (!mysql_stmt_fetch(stmt))
		{
			row_count++;
			// printf("token dump:  token<%s>, type<%s>, value<%i>\n", t->token, t->ttype, t->value);
		}
		if (row_count == 0) return NULL;

		/* Validate rows fetched */
		fprintf(stdout, " total rows fetched: %d\n", row_count);
		if (row_count != 1)
		{
			fprintf(stderr, " MySQL returmed too many rows\n");
			exit(0);
		}
	}

	/* Free the prepared result metadata */
	// puts("freeing metadata\n");
	mysql_free_result(prepare_meta_result);

	/* Close the statement */
	// puts("closing statement\n");
	if (mysql_stmt_close(stmt))
	{
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	/* delete oldest entrty of the queue  */
	sprintf(buff, "delete from TokenQ WHERE tokenID = '%i';", tokenID);
	if (mysql_query(mysql, buff))
		show_mysql_error(mysql);

	// printf("connection id: %ld\n", mysql_thread_id(mysql));
	puts("closing connection\n");
	mysql_close(mysql);

	mysql_library_end();

	return t;
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
	char 			tbuf[_LINE_BUFFER_SIZE];


	tbuf_ptr = tbuf;
	lbuf_ptr = lbuf;
	memset(tbuf, '\0', _LINE_BUFFER_SIZE);

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
