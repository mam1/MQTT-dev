
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tq"
#define USER        "test-sql"
#define PSWD        "test-sql"

static void show_mysql_error(MYSQL *mysql)
{
	printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
	       mysql_sqlstate(mysql),
	       mysql_error(mysql));
	exit(-1);
}

int main(int argc, char* argv[]) {
	// MYSQL         place;
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_FIELD         *field;
	MYSQL_ROW           row;

	int                 i;

	/* get handles  */
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("couldn't initialize mysql: %s\n", mysql_error(conn));
		exit(1);
	}

	/* initailze client library */
	if (mysql_library_init(argc, argv, NULL)) {
		printf("couldn't initialize MySQL client library\n");
		exit(1);
	}

	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("couldn't connect to database\n");
		exit(1);
	}

	if (mysql_query(conn, "SELECT Channels.* FROM Channels WHERE enabled = 'yes'"))
		show_mysql_error(conn);

	result = mysql_store_result(conn);
	row = mysql_fetch_row(result);

	while (row != NULL) {
		printf("%s\n","**************************************************" );		
		for (i = 0; i < (int)mysql_num_fields(result); i++) {  
			mysql_field_seek(result, i);
			field = mysql_fetch_field(result);
			printf("	column %i <%s> \t%s .... %s\n", i, field->name, row[i]);
		}
		printf("%s\n","**************************************************" );		
	}
	mysql_free_result(result);
	mysql_close(conn);
	printf("%s\n", "normal termination");
}
