
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


int Tpush(char * token)
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

	row = mysql_fetch_row(result);
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
		while ((*lbuf_ptr != ' ') AND (*lbuf_ptr != '\0')) *tbuf++ = *lbuf_ptr++;
		Tpush(tbuff);
	}
	return 1;
}


int main(int argc, char* argv[]) {
	char 			tbuf[500];

	tokenizer("  this is a test");

	memset(tbuf, '\0', 500);
	Tpop(tbuf);
	printf("popped token <%s>\n\n", tbuf );
	printf("%s\n", "normal termination");
}
