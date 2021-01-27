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

static void show_mysql_error(MYSQL *mysql)
{
  printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
                                  mysql_sqlstate(mysql),
                                  mysql_error(mysql));
  exit(-1);
}

static void show_stmt_error(MYSQL_STMT *stmt)
{
  printf("Error(%d) [%s] \"%s\"\n", mysql_stmt_errno(stmt),
                                  mysql_stmt_sqlstate(stmt),
                                  mysql_stmt_error(stmt));
  exit(-1);
}

static void load_channels(void){
  MYSQL *mysql = NULL;
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[3];

  /* initialize client library */
  if (mysql_library_init(0, NULL, NULL)) {
      printf("could not initialize MySQL client library\n");
      exit(1);
  }
  printf("%s\n", "  client library intialized");

  /* connect to MariaDB server */
  mysql= mysql_init(NULL);
  if (!mysql_real_connect(mysql, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, 0))
      show_mysql_error(mysql);
  printf("  connection to %s established\n", SERVER);

  /* Data for insert */
  printf("  building data for insert\n");
  const char *name[]= {"test channel 1", "test channel 2", "test channel 3"};
  unsigned long name_length[]= {14,14,14};
  const int *active[]= {0,0,0};

  char name_ind[]= {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_DEFAULT}; 
  char id_ind[]= {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};

  unsigned int array_size = 3; 

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

  memset(bind, 0, sizeof(MYSQL_BIND) * 3);

  bind[0].u.indicator= id_ind;
  bind[0].buffer_type= MYSQL_TYPE_LONG;

  bind[1].buffer= name;
  bind[1].buffer_type= MYSQL_TYPE_STRING;
  bind[1].length= name_length;

  bind[2].buffer= active;
  bind[2].buffer_type= MYSQL_TYPE_INT;

 /* set array size */
  if(mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size))
    show_stmt_error(stmt);
  printf("%s\n", "  array size set");

  /* bind parameter */
  if(mysql_stmt_bind_param(stmt, bind))
    show_stmt_error(stmt);

  printf("%s\n", "  bind");

  /* execute */
  if (mysql_stmt_execute(stmt))
    show_stmt_error(stmt);

  mysql_stmt_close(stmt);
  mysql_library_end();
  mysql_close(mysql);
  mysql_library_end();

  printf("%s\n", "test channel data loaded\n");
  
}

int main(int argc, char *argv[])
{
  printf("%s\n","\n*******************************************************************" );
  printf("%s\n", "\ntest data loader\n");
  printf("   SERVER   %s\n",SERVER);
  printf("   SOCKETT  %s\n",SOCKETT);
  printf("   DATABASE %s\n",DATABASE);
  printf("   USER     %s\n",USER);
  printf("   PSWD     %s\n\n",PSWD);
  
  load_channels();

  printf("%s\n", "normal termination");

}