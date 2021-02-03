#include <stdio.h>
#include <string.h>
#include <mysql.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "test-schedule"
#define USER        "test-sql"
#define PSWD        "test-sql"

int main(int argc, char *argv[])
{
   MYSQL *conn;
   MYSQL_STMT *stmt;

   int id[2];
   char id_ind[2];
   char *nameptr[2];
   char name[2][256];
   char name_ind[2];
   long name_len[2];
   MYSQL_TIME *regdateptr[2];
   MYSQL_TIME regdate[2];
   char regdate_ind[2];
   int numorders[2];
   char numorders_ind[2];
   MYSQL_BIND bind[4];
   unsigned int numrows;

  printf("%s\n","\n*******************************************************************" );
  printf("%s\n", "\ntest data loader\n");
  printf("   SERVER   %s\n",SERVER);
  printf("   SOCKETT  %s\n",SOCKETT);
  printf("   DATABASE %s\n",DATABASE);
  printf("   USER     %s\n",USER);
  printf("   PSWD     %s\n\n",PSWD);

/* connect to server */
   conn = mysql_init(NULL);
   if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
   {
      fprintf(stderr, "Error: %s\n", mysql_error(conn));
      return 1;
   }

/* setup statement */
   stmt = mysql_stmt_init(conn);
   if (mysql_stmt_prepare(stmt, "INSERT INTO customers VALUES(?, ?, ?, ?)", -1) != 0)
   {

      fprintf(stderr, "Error: %s\n", mysql_stmt_error(stmt));
      return 1;
   }

/* initialize bind array */
   memset(bind, 0, sizeof(bind));

   bind[0].u.indicator = id_ind;
   bind[0].buffer_type = MYSQL_TYPE_LONG;
   bind[0].buffer = id;

   bind[1].u.indicator = name_ind;
   bind[1].buffer_type = MYSQL_TYPE_STRING;
   bind[1].buffer = nameptr;
   bind[1].length = name_len;
   bind[2].u.indicator = regdate_ind;
   bind[2].buffer_type = MYSQL_TYPE_DATETIME;
   bind[2].buffer = regdateptr;
   bind[3].u.indicator = numorders_ind;
   bind[3].buffer_type = MYSQL_TYPE_LONG;
   bind[3].buffer = numorders;

   if (mysql_stmt_bind_param(stmt, bind) != 0)

   {
      fprintf(stderr, "Error: %s\n", mysql_stmt_error(stmt));
      return 1;
   }

   memset(&regdate, 0, sizeof(regdate));

   id_ind[0] = regdate_ind[0] = numorders_ind[0] = STMT_INDICATOR_NONE;
   name_ind[0] = STMT_INDICATOR_NTS;
   id[0] = 1;
   nameptr[0] = name[0];
   strcpy(name[0], "Joe Bloggs");
   name_len[0] = 256;
   regdateptr[0] = &regdate[0];
   regdate[0].year = 2019;
   regdate[0].month = 3;

   regdate[0].day = 5;

   regdate[0].hour = 14;
   regdate[0].minute = 30;

   numorders[0] = 0;


   id_ind[1] = regdate_ind[1] = numorders_ind[1] = STMT_INDICATOR_NONE;
   name_ind[1] = STMT_INDICATOR_NTS;
   id[1] = 2;
   nameptr[1] = name[1];
   strcpy(name[1], "Homer Simpson");
   name_len[1] = 256;
   regdateptr[1] = &regdate[1];
   regdate[1].year = 2019;
   regdate[1].month = 4;
   regdate[1].day = 12;
   regdate[1].hour = 9;
   regdate[1].minute = 15;
   numorders[1] = 0;

   numrows = 2;
   mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &numrows);
   if (mysql_stmt_execute(stmt) != 0)
   {
      fprintf(stderr, "Error: %s\n", mysql_stmt_error(stmt));
      return 1;
   }
   mysql_close(conn);
   return 0;



}

