
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

MYSQL               *conn;
MYSQL_RES           *result;
MYSQL_FIELD         *field;
MYSQL_ROW           row;

int 				i;
int 				rows_returned;
int 				offset;

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

void test(char * offset)
{



}

int main(int argc, char* argv[])
{
	char 			cnum[20];
	char 			offset[20];
	char 			buff[2000];

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
	printf("enter channel number >");
	fgets(cnum,1,stdin);
	printf("enter offset >");
	fgets(offset,1,stdin);

	sprintf(buff, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset = %s ", offset);
	if (mysql_query(conn, buff)) show_mysql_error(conn);
	result = mysql_store_result(conn);
	rows_returned = (int)mysql_num_rows(result);
	printf("%i rows returned from = query\n", rows_returned);

	if (rows_returned == 1)
	{
		row = mysql_fetch_row(result);
		printf("hit\n");
		printf("set channel state to %s\n", row[10]);
		mysql_free_result(result);
	}

	else
	{
		mysql_free_result(result);
		sprintf(buff, "SELECT Channels.*, Transitions.*, Schedules.* FROM Channels JOIN Transitions USING(scheduleID) INNER JOIN Schedules USING(scheduleID) WHERE Channels.scheduleID = Transitions.scheduleID AND Transitions.transition_offset < %s ", offset);
		if (mysql_query(conn, buff)) show_mysql_error(conn);
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
		mysql_free_result(result);
	}

mysql_close(conn);
printf("%s\n", "normal termination");
}
