
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

int get_offset(int day, int hour, int minute) {
	int 		off;
	off = minute + (hour * MINUTES_PER_HOUR) + (day * MINUTES_PER_DAY);
	return off;
}

void update_channel_state(void) {
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Current local time and date: %s", asctime (timeinfo) );
	printf("day of the week %i\n", timeinfo->tm_wday);
	printf("get_offset = %i\n", get_offset(timeinfo->tm_wday,timeinfo->tm_hour,timeinfo->tm_min));
	// printf("testing channel %s offset %i ........ new state %s\n", );



	// time_t			t;
	// int 			offset;
	// time(&t);
	// // offset = get_offset(t.tm_wday, t.tm_hour, t.tm_min);
	// printf("\nThis program has been writeen at (date and time): %s", ctime(&t));
	// printf("offset = %i\n", offset );

	return;
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

	// int                 i;

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

	// if (mysql_query(conn, "SELECT Channels.*,Schedules.* FROM Channels C INNER JOIN Schedules S USING(scheduleID) WHERE C.enabled = 'yes'"))
	if (mysql_query(conn, "SELECT C.Channels.*,S.Schedules.* FROM Channels C INNER JOIN Schedules S USING(scheduleID) WHERE C.enabled = 'yes'"))
		show_mysql_error(conn);
	result = mysql_store_result(conn);

	while ((row = mysql_fetch_row(result)) != NULL) {
		mysql_field_seek(result, 7);
		field = mysql_fetch_field(result);
		printf("channel %s \n",  row[2] );
/********************************************************************/		






/********************************************************************/		
	}
	mysql_free_result(result);
	mysql_close(conn);
	printf("%s\n", "normal termination");
}
