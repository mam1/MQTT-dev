
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>
#include <time.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tq"
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

int offset(int day, int hour, int minute){
	int 		off;
	off = minute + (hour*MINUTES_PER_HOUR) + (day*MINUTES_PER_DAY);
	return off;
}

char * get_channel_state(time_t t){

	char 	ret[]={"on"};
	return ret;
}

int main(int argc, char* argv[]) {
	// MYSQL         place;
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_FIELD         *field;
	MYSQL_ROW           row;



	       // int    tm_sec   Seconds [0,60].
        //    int    tm_min   Minutes [0,59].
        //    int    tm_hour  Hour [0,23].
        //    int    tm_mday  Day of month [1,31].
        //    int    tm_mon   Month of year [0,11].
        //    int    tm_year  Years since 1900.
        //    int    tm_wday  Day of week [0,6] (Sunday =0).
        //    int    tm_yday  Day of year [0,365].
        //    int    tm_isdst Daylight Savings flag.

	int                 i;

	/* currtent time */
	time_t 			t;

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

	if (mysql_query(conn, "SELECT * FROM Channels WHERE enabled = 'yes'"))
		show_mysql_error(conn);
	result = mysql_store_result(conn);






	
	printf("number of rows effected %i\n", (int)mysql_num_rows(result) );
	printf("number of fields %i\n", (int)mysql_num_fields(result));

	while ((row = mysql_fetch_row(result)) != NULL) {
		printf("%s\n","**************************************************" );		
		for (i = 0; i < (int)mysql_num_fields(result); i++) {
			mysql_field_seek(result, i);
			field = mysql_fetch_field(result);
			printf("	column %i <%s> \t%s\t", i, field->name, row[i]);
			t = time(NULL);
			printf("channel %s should be %s\n", field->name, get_channel_state(time_t t) );

		}
		printf("%s\n","**************************************************" );		
	}
	mysql_free_result(result);
	mysql_close(conn);
	printf("%s\n", "normal termination");
}
