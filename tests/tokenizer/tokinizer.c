
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

	// /* initailze client library */
	// if (mysql_library_init(argc, argv, NULL))
	// {
	// 	printf("couldn't initialize MySQL client library\n");
	// 	exit(1);
	// }

	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("couldn't connect to database\n");
		exit(1);
	}

	/* position curser at newest token */
	if (mysql_query(conn, "SELECT * FROM TokenQ ORDER BY tokenID LIMIT 1;"))
		show_mysql_error(conn);

	/* insert newest token */
		// printf("token = <%s>\n", *token);
	// sprintf(buff, "INSERT INTO TokenQ(token) VALUES(%s);", *token);
	// if (mysql_query(conn, buff))
		show_mysql_error(conn);
	return 1;
}

char * Tpop(char * token)
{
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_FIELD         *field;
	MYSQL_ROW           row;

	/* get handles  */
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("couldn't initialize conn: %s\n", mysql_error(conn));
		exit(1);
	}

	// /* initailze client library */
	// if (mysql_library_init(argc, argv, NULL))
	// {
	// 	printf("couldn't initialize MySQL client library\n");
	// 	exit(1);
	// }

	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("couldn't connect to database\n");
		exit(1);
	}
	return token;
}
int main(int argc, char* argv[]) {
	printf("%s\n", "testing push");
	Tpush("from c");
	printf("%s\n", "normal termination");
}
