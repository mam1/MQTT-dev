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

// #define INSERT_ROWS          4

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

//   MYSQL mysql;
//   MYSQL_STMT *stmt;
//   MYSQL_BIND bind[INSERT_ROWS];
//   // MYSQL_RES *result[INSERT_ROWS];

// int main(int argc, char *argv[])
// {
//   MYSQL   *database;
//   database = &mysql

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
//   mysql= mysql_init(mysql);
//   if (!mysql_real_connect(mysql, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, 0))
//       show_mysql_error(mysql);
//   printf("  connection to %s established\n", SERVER);

//   /* create channels table */
//   if (mysql_query(mysql, "DROP TABLE IF EXISTS channels"))
//       show_mysql_error(mysql);

//   if (mysql_query(mysql, "CREATE TABLE channels (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,active INT NOT NULL DEFAULT 0, name CHAR(30))"))
//     show_mysql_error(mysql);
//   printf("%s\n", "  table <channels> created" );

//   /* Data for insert */
//   printf("  building data for insert\n");
//   const char *name[] = {"test channel 1", "test channel 2", "test channel 3", "test channel 4"};
//   unsigned long name_length[] = {14,14,14,14};
//   unsigned long active[] = {0,1,0,1};

//   char id_ind[] = {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};
//   // char name_ind[] = {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_NULL}; 
//   // char active_ind[] = {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_DEFAULT};

//   unsigned int array_size = INSERT_ROWS; 

// /* get a handle to statement structure */
//   stmt = mysql_stmt_init(mysql);
//   if (stmt) {
//         puts("  statement init OK");
//       } else {
//         printf("statement init failed: %s\n", mysql_error(mysql));
//       }

//   if (mysql_stmt_prepare(stmt, "INSERT INTO channels VALUES (?,?,?)", -1))
//     show_stmt_error(stmt);

//   printf("%s\n", "  statement prepared");

//   memset(bind, 0, sizeof(MYSQL_BIND) * INSERT_ROWS);

//   bind[0].u.indicator= id_ind;
//   bind[0].buffer_type= MYSQL_TYPE_LONG;

//   bind[2].buffer= name;
//   bind[2].buffer_type= MYSQL_TYPE_STRING;
//   bind[2].length= name_length;

//   bind[1].buffer= active;
//   bind[1].buffer_type= MYSQL_TYPE_LONG;

//  /* set array size */
//   if(mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size))
//     show_stmt_error(stmt);
//   printf("%s\n", "  array size set");

//   /* bind parameter */
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
/*
 * mysql prepared statement segfault code sample
 *
 */



#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>

/* in the sql table inode is an int unsigned and rpath is a varchar */
#define SAMPLE_QUERY "select name from channels"

int main(int argc, char* argv[]) {
  MYSQL         place;
  MYSQL*        sql;
  MYSQL_STMT*   stmt;
  MYSQL_BIND    bind[1];
  MYSQL_BIND    result[1];
  unsigned long length[1];
  my_bool       is_null[1];
  my_bool       error[1];
  char*         rpath = "/";
  unsigned long used;
  unsigned long res = 0;
  int           err;

  sql = mysql_init(&place);
  if (sql == NULL) {
    fprintf(stderr, "mysql failed to initialize\n");
    exit(0);
  }
  mysql_ssl_set(sql, NULL, NULL, NULL, NULL, NULL);
  sql = mysql_real_connect(sql, "localhost", "test-sql", "test-sql", "test-schedule", 0, NULL, 0);
  if (sql == NULL) {
    fprintf(stderr, "couldn't connect to database: %s\n", mysql_error(&place));
    exit(0);
  }
  
  stmt = mysql_stmt_init(sql);
  if (stmt == NULL) {
    fprintf(stderr, "couldn't initialize statement: %s\n", mysql_error(&place));
    exit(0);
  }
  err = mysql_stmt_prepare(stmt, SAMPLE_QUERY, sizeof(SAMPLE_QUERY)-1);
  if (err != 0) {
    fprintf(stderr, "statement prepare failed: %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  used = strlen(rpath)+1; /* want the trailing null char */
  memset(bind, 0, sizeof (bind));
  bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
  bind[0].buffer = rpath;
  bind[0].is_null = 0;
  bind[0].length = &used;

  err = mysql_stmt_bind_param(stmt, bind);
  if (err != 0) {
    fprintf(stderr, "parameter bind failed: %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  err = mysql_stmt_execute(stmt);
  if (err != 0) {
    fprintf(stderr, "statement(1) execute failed: %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  result[0].buffer_type = MYSQL_TYPE_LONG;
  result[0].buffer = &res;
  result[0].length = &length[0];
  result[0].is_null = &is_null[0];
  result[0].error = &error[0];
  err = mysql_stmt_bind_result(stmt, result);
  if (err != 0) {
    fprintf(stderr, "result bind failed: %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  err = mysql_stmt_fetch(stmt);
  if (err != 0) {
    fprintf(stderr, "fetch(1) failed: %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  mysql_stmt_free_result(stmt);

  fprintf(stderr, "res = %lu,  sleeping 30 seconds, reset mysql server now\n", res);
  sleep(30);
  err = mysql_stmt_execute(stmt);
  if (err != 0) { /* expect the reset connection message here */
    fprintf(stderr, "statement(2) execute failed: %s\n", mysql_stmt_error(stmt));
  } else {
    err = mysql_stmt_fetch(stmt);
    mysql_stmt_free_result(stmt);
  }
  err = mysql_stmt_execute(stmt); /* expect segfault here */
  if (err != 0) {
    fprintf(stderr, "statement(3) execute failed: %s\n", mysql_stmt_error(stmt));
  } else {
    err = mysql_stmt_fetch(stmt);
    mysql_stmt_free_result(stmt);
  }
  printf("%s\n", "nothing failed");
}
