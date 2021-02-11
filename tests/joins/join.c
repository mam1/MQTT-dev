
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

int get_offset(int day, int hour, int minute)
{
	int 		off;
	off = minute + (hour * MINUTES_PER_HOUR) + (day * MINUTES_PER_DAY);
	return off;
}

void update_channel_state(void)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf ( "Current local time and date: %s", asctime (timeinfo) );
	printf("day of the week %i\n", timeinfo->tm_wday);
	printf("get_offset = %i\n", get_offset(timeinfo->tm_wday, timeinfo->tm_hour, timeinfo->tm_min));

	return;
}

int main(int argc, char* argv[])
{
	MYSQL               *conn;
	MYSQL_RES           *result;
	MYSQL_FIELD         *field;
	MYSQL_ROW           row;

	int 				i;
	int 				rows_returned;
	int 				offset;

	/* get handles  */
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("couldn't initialize conn: %s\n", mysql_error(conn));
		exit(1);
	}

	/* initailze client library */
	if (mysql_library_init(argc, argv, NULL))
	{
		printf("couldn't initialize MySQL client library\n");
		exit(1);
	}

	/* connect to server */
	if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
	{
		printf("couldn't connect to database\n");
		exit(1);
	}
	// if (mysql_query(conn, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset > 410  AND Transitions.transition_offset < 800")) show_mysql_error(conn);
	// if (mysql_query(conn, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset > 410  AND Transitions.transition_offset < 800")) show_mysql_error(conn);

	if (mysql_query(conn, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset = 410 ")) show_mysql_error(conn);
	result = mysql_store_result(conn);
	rows_returned = (int)mysql_num_rows(result);
	printf("%i rows returned\n", rows_returned);


	if (rows_returned == 1)
	{
		row = mysql_fetch_row(result);
		// mysql_field_seek(result, 6);
		// field = mysql_fetch_field(result);
		printf("hit\n");
		printf("set channel state to %s\n", row[10]);
		mysql_free_result(result);
	}

	else
	{
		mysql_free_result(result);
		if (mysql_query(conn, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset < 410  ORDER BY Transitions.scheduleID ASC, Transitions.offset ASC")) show_mysql_error(conn);
		result = mysql_store_result(conn);
		rows_returned = (int)mysql_num_rows(result);
		printf("%i rows returned\n", rows_returned);

		if (rows_returned == 0)
		{
			printf("no change to channel state\n");
		}
		else
		{
			row = mysql_fetch_row(result);
			printf("set channel state to %s\n", row[10]);
		}
		// mysql_data_seek(result, 0);
		// mysql_field_seek(result, 6);
		// field = mysql_fetch_field(result);



		// printf(" %i  active columns,  ", mysql_num_fields(result));
		// printf("%i rows returned\n", (rows_returned);
		// for (i = 0; i < (int)mysql_num_fields(result); i++)
		// {
		// 	mysql_field_seek(result, i);
		// 	field = mysql_fetch_field(result);
		// 	printf("column %i  %s\n", i, field->name);
		// }
		// printf("\n");
		mysql_free_result(result);
	}



// mysql_data_seek(result, 0);
// while ((row = mysql_fetch_row(result)) != NULL)
// {
// 	for (i = 0; i < (int)mysql_num_fields(result); i++)
// 	{
// 		mysql_field_seek(result, i);
// 		field = mysql_fetch_field(result);
// 		printf("  column %i \t<%s>\n", i, row[i]);
// 	}
// 	printf("\n");

// }
// mysql_free_result(result);
mysql_close(conn);
printf("%s\n", "normal termination");
}
