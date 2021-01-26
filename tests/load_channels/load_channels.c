/* 
    load channel data
*/

#include "mysql.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "example"
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

void load_channels(void){
  MYSQL *mysql;
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[3];

  /* Data for insert */
  const char *name[]= {"test channel 1", "test channel 2", "test channel 3"};
  unsigned long name_length[]= {14,14,14};
  const bool *active[]= {0,0,0}

  char name_ind[]= {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_DEFAULT}; 
  char id_ind[]= {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};

  unsigned int array_size= 3; 

  mysql= mysql_init(NULL);

   /* connect to MariaDB server */
  if (!mysql_real_connect(mysql, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, 0))
    show_mysql_error(mysql);

  printf("connection to %s established", SERVER);

  stmt= mysql_stmt_init(mysql);
  if (mysql_stmt_prepare(stmt, "INSERT INTO channels VALUES (?,?,?)", -1))
    show_stmt_error(stmt);

  memset(bind, 0, sizeof(MYSQL_BIND) * 3);



  bind[0].u.indicator= id_ind;
  bind[0].buffer_type= MYSQL_TYPE_LONG;

  bind[1].buffer= forenames;
  bind[1].buffer_type= MYSQL_TYPE_STRING;
  bind[1].u.indicator= forename_ind;

  bind[2].buffer_type= MYSQL_TYPE_STRING;
  bind[2].buffer= surnames;
  bind[2].length= &surnames_length;


  

}

int main(int argc, char *argv[])
{

  printf("%s\n", "\ntest data loader\n");
  printf("%s\n", );

  printf("   SERVER   %s\n",SERVER);
  printf("   SOCKETT  %s\n",SOCKETT);
  printf("   DATABASE %s\n",DATABASE);
  printf("   USER     %s\n",USER);
  printf("   PSWD     %s\n\n",PSWD);
  



  /* We autogenerate id's, so all indicators are STMT_INDICATOR_NULL */


  /* set array size */
  mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size);

  /* bind parameter */
  mysql_stmt_bind_param(stmt, bind);

  /* execute */
  if (mysql_stmt_execute(stmt))
    show_stmt_error(stmt);

  mysql_stmt_close(stmt);
  mysql_close(mysql);

  printf("%s\n", "made it this far!!!!\n");
}