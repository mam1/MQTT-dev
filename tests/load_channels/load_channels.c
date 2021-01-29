/* 
    load channel test data
*/

#include "mysql.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "test-schedule"
#define USER        "test-sql"
#define PSWD        "test-sql"

#define INSERT_ROWS     4
#define FIELD_COUNT     3
#define FIELD_NAME      "channels"

static void show_mysql_error(MYSQL *mysql)
{
  printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
                                  mysql_sqlstate(mysql),
                                  mysql_error(mysql));
  exit(-1);
}

void show_stmt_error(MYSQL_STMT *stmt)
{
  printf("Error(%d) [%s] \"%s\"\n", mysql_stmt_errno(stmt),
                                  mysql_stmt_sqlstate(stmt),
                                  mysql_stmt_error(stmt));
  exit(-1);
}

  // MYSQL_RES *result[INSERT_ROWS];

int main(int argc, char *argv[])
{
  MYSQL           place;
  MYSQL           *mysql;
  MYSQL_STMT      *stmt;
  MYSQL_BIND      bind[FIELD_COUNT];


  printf("%s\n","\n*******************************************************************" );
  printf("%s\n", "\ntest data loader\n");
  printf("   SERVER   %s\n",SERVER);
  printf("   SOCKETT  %s\n",SOCKETT);
  printf("   DATABASE %s\n",DATABASE);
  printf("   USER     %s\n",USER);
  printf("   PSWD     %s\n\n",PSWD);

/* initialize client library */
  if (mysql_library_init(0, NULL, NULL)) {
      printf("could not initialize MySQL client library\n");
      exit(1);
  }
  printf("%s\n", "  client library intialized");

/* connect to MariaDB server */
  mysql= mysql_init(&place);
  if (!mysql_real_connect(mysql, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, 0))
      show_mysql_error(mysql);
  printf("  connection to %s established\n", SERVER);

  /* create table */
  if (mysql_query(mysql, "DROP TABLE IF EXISTS channels"))
      show_mysql_error(mysql);

  if (mysql_query(mysql, "CREATE TABLE channels (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,active INT NOT NULL DEFAULT 0, name CHAR(30))"))
    show_mysql_error(mysql);
  printf("  table %s created\n","channels" );

  /* initialize data for insert */
  printf("  building data for insert\n");
  const char *name[] = {"test channel 1", "test channel 2", "test channel 3", "test channel 4"};
  unsigned long name_length[] = {15,15,15,15};
  unsigned long active[] = {0,1,0,1};

  char id_ind[] = {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};
  // char name_ind[] = {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_NULL}; 
  char active_ind[] = {STMT_INDICATOR_NULL, STMT_INDICATOR_DEFAULT, STMT_INDICATOR_NTS};

  unsigned int array_size = FIELD_COUNT; 

/* get a handle to statement structure */
  stmt = mysql_stmt_init(mysql);
  if (stmt) {
        puts("  statement init OK");
      } else {
        printf("statement init failed: %s\n", mysql_error(mysql));
      }

  if (mysql_stmt_prepare(stmt, "INSERT INTO channels VALUES (?,?,?)", -1))
    show_stmt_error(stmt);

  printf("%s\n", "  statement prepared");

  memset(bind, 0, sizeof(MYSQL_BIND) * FIELD_COUNT);

  bind[0].u.indicator = id_ind;
  bind[0].buffer_type = MYSQL_TYPE_LONG;

  bind[1].buffer = active;
  bind[1].buffer_type = MYSQL_TYPE_LONG;

  bind[2].buffer = name;
  bind[2].buffer_type = MYSQL_TYPE_STRING;
  bind[2].length = name_length;

 /* set array size */
  if(mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size))
    show_stmt_error(stmt);
  printf("  array size set to %i\n", array_size);

  /* bind parameter */
  if(mysql_stmt_bind_param(stmt, bind))
    show_stmt_error(stmt);
  printf("%s\n", "  bind");

  /* execute */
  if (mysql_stmt_execute(stmt))
    show_stmt_error(stmt);
  mysql_stmt_close(stmt);
  printf("%s\n", "test channel data loaded\n");

  /* clean up */
  mysql_library_end();
  mysql_close(mysql);
  printf("%s\n", "normal termination");

}
