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

  printf("%s\n", "\n*******************************************************************" );
  printf("%s\n", "\ntest data loader\n");
  printf("   SERVER   %s\n", SERVER);
  printf("   SOCKETT  %s\n", SOCKETT);
  printf("   DATABASE %s\n", DATABASE);
  printf("   USER     %s\n", USER);
  printf("   PSWD     %s\n\n", PSWD);

  /* get handles  */
  conn = mysql_init(NULL);
  stmt = mysql_stmt_init(conn);

  /* connect to server */
  if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
  {
    fprintf(stderr, "Error: %s\n", mysql_error(conn));
    return 1;
  }

  /* create a clean table */
  if (mysql_query(mysql, "DROP TABLE IF EXISTS channels"))
    {
    fprintf(stderr, "Error: %s\n", mysql_error(conn));
    return 1;
  }

  if (mysql_query(mysql, "CREATE TABLE channels (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,active INT NOT NULL DEFAULT 0, name CHAR(30))"))
    show_mysql_error(mysql);
  printf("  table %s created\n", "channels" );



  if (mysql_stmt_prepare(stmt, "CREATE TABLE customers(id INTEGER NOT NULL PRIMARY KEY, cust_name VARCHAR(255), cust_regdate DATETIME NOT NULL, cust_numorders INTEGER NOT NULL)", -1) != 0)
  {

    fprintf(stderr, "Error: %s\n", mysql_stmt_error(stmt));
    return 1;
  }





  if (/* condition */)
  {
    /* code */
  }

  /* setup statement */

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








// /*
//     load channel test data
// */

// #include "mysql.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// #define SERVER      "localhost"
// #define SOCKETT     "/run/mysqld/mysqld.sock"
// #define DATABASE    "test-schedule"
// #define USER        "test-sql"
// #define PSWD        "test-sql"

// #define INSERT_ROWS     4
// #define FIELD_COUNT     3
// #define FIELD_NAME      "channels"

// static void show_mysql_error(MYSQL *mysql)
// {
//   printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
//                                   mysql_sqlstate(mysql),
//                                   mysql_error(mysql));
//   exit(-1);
// }

// void show_stmt_error(MYSQL_STMT *stmt)
// {
//   printf("Error(%d) [%s] \"%s\"\n", mysql_stmt_errno(stmt),
//                                   mysql_stmt_sqlstate(stmt),
//                                   mysql_stmt_error(stmt));
//   exit(-1);
// }

//   // MYSQL_RES *result[INSERT_ROWS];

// int main(int argc, char *argv[])
// {
//   MYSQL           place;
//   MYSQL           *mysql;
//   MYSQL_STMT      *stmt;
//   MYSQL_BIND      bind[3];      /********************************************************************************************/

//   /* initialize data for insert */
//   const char *name[] = {"test channel 1", "test channel 2", "test channel 3", "test channel 4"};
//   unsigned long name_length[] = {15,15,15,15};
//   unsigned long active[] = {0,1,0,1};


//   char id_ind[] = {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};
//   // char name_ind[] = {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_NULL};
//   // char active_ind[] = {STMT_INDICATOR_NULL, STMT_INDICATOR_DEFAULT, STMT_INDICATOR_NTS};

//   unsigned int array_size = 4;                                  /**************************************************************/



//   printf("%s\n","\n*******************************************************************" );
//   printf("%s\n", "\ntest data loader\n");
//   printf("   SERVER   %s\n",SERVER);
//   printf("   SOCKETT  %s\n",SOCKETT);
//   printf("   DATABASE %s\n",DATABASE);
//   printf("   USER     %s\n",USER);
//   printf("   PSWD     %s\n\n",PSWD);

// /* initialize client library */
//   if (mysql_library_init(0, NULL, NULL)) {
//       printf("could not initialize MySQL client library\n");
//       exit(1);
//   }
//   printf("%s\n", "  client library intialized");

// /* connect to MariaDB server */
//   mysql = mysql_init(NULL);
//   if (!mysql_real_connect(mysql, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, 0))
//       show_mysql_error(mysql);
//   printf("  connection to %s established\n", SERVER);

// /* create table */
//   if (mysql_query(mysql, "DROP TABLE IF EXISTS channels"))
//       show_mysql_error(mysql);
//   if (mysql_query(mysql, "CREATE TABLE channels (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,active INT NOT NULL DEFAULT 0, name CHAR(30))"))
//     show_mysql_error(mysql);
//   printf("  table %s created\n","channels" );

// /* get a handle to a statement structure */
//   stmt = mysql_stmt_init(mysql);
//   if (stmt) {
//         puts("  statement init OK");
//       } else {
//         printf("statement init failed: %s\n", mysql_error(mysql));
//       }
//   if (mysql_stmt_prepare(stmt, "INSERT INTO channels VALUES (?,?,?)", -1))
//     show_stmt_error(stmt);
//   printf("%s\n", "  statement prepared");

//   memset(bind, 0, sizeof(MYSQL_BIND) * 3);     /***************************************************************************/

//   bind[0].u.indicator = id_ind;
//   bind[0].buffer_type = MYSQL_TYPE_LONG;

//   bind[1].buffer = active;
//   bind[1].buffer_type = MYSQL_TYPE_LONG;

//   bind[2].buffer = name;
//   bind[2].buffer_type = MYSQL_TYPE_STRING;
//   bind[2].length = &name_length;

//  /* set array size */
//   if(mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size))
//     show_stmt_error(stmt);
//   printf("  array size set to %i\n", array_size);

// /* bind parameter */
//   if(mysql_stmt_bind_param(stmt, bind))
//     show_stmt_error(stmt);
//   printf("%s\n", "  bind");

//   /* execute */
//   if (mysql_stmt_execute(stmt))
//     show_stmt_error(stmt);
//   mysql_stmt_close(stmt);
//   printf("%s\n", "test channel data loaded\n");

//   /* clean up */
//   mysql_library_end();
//   mysql_close(mysql);
//   printf("%s\n", "normal termination");

// }
