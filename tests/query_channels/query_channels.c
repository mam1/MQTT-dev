
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "test-schedule"
#define USER        "test-sql"
#define PSWD        "test-sql"

/* in the sql table inode is an int unsigned and rpath is a varchar */
#define SAMPLE_QUERY "select * from channels"

int main(int argc, char* argv[]) {
  MYSQL         place;
  MYSQL*        mysql;
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


/* get handles  */
  conn = mysql_init(NULL);
  if (stmt == NULL) {
    fprintf(stderr, "couldn't initialize mysql: %s\n", mysql_error(conn));
    exit(0);
  }
  // stmt = mysql_stmt_init(conn);
  // if (stmt == NULL) {
  //   fprintf(stderr, "couldn't initialize statement: %s\n", mysql_error(conn));
  //   exit(0);
  // }

/* initailze client library */
  if (mysql_library_init(argc, argv, NULL)) {
    fprintf(stderr, "could not initialize MySQL client library\n");
    exit(1);
  }

/* connect to server */
  if (mysql_real_connect(conn, SERVER, USER, PSWD, DATABASE, 0, SOCKETT, CLIENT_INTERACTIVE) == NULL)
  {
    fprint("couldn't connect ot database\n",);
    fprintf(stderr, "Error: %s\n", mysql_error(conn));
    return 1;
  }
  

  if (mysql_query(mysql, "SELECT * FROM channels"))
    show_mysql_error(mysql);
  
  // mysql_stmt_free_result(stmt);
  mysql_close(conn);
  printf("%s\n", "normal termination");
}
