
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
	// MYSQL_RES           *result;
	// MYSQL_FIELD         *field;
	// MYSQL_ROW           row;

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


	char 				buff[500];
	
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
	sprintf(buff, "SELECT * FROM TokenQ ORDER BY tokenID LIMIT 1", token);
	if (mysql_query(conn, buff))
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


	return "*****";
}
int main(int argc, char* argv[]) {
	char 			tbuff[500];
	printf("%s\n", "testing push");
	Tpush("from c");
	Tpop(tbuff);
	printf("popped token %s\n\n", tbuff );
	printf("%s\n", "normal termination");
}
