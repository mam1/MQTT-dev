
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>

#define SERVER      "localhost"
#define SOCKETT     "/run/mysqld/mysqld.sock"
#define DATABASE    "tq"
#define USER        "test-sql"
#define PSWD        "test-sql"




static void show_mysql_error(MYSQL *mysql)
{
  printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
                                  mysql_sqlstate(mysql),
                                  mysql_error(mysql));
  exit(-1);
}

int main(int argc, char* argv[]) {
  // MYSQL         place;
  MYSQL*        conn;
  // MYSQL_STMT*   stmt;
  // MYSQL_BIND    bind[1];
  // MYSQL_BIND    result[1];
  // unsigned long length[1];
  // my_bool       is_null[1];
  // my_bool       error[1];
  // char*         rpath = "/";
  // unsigned long used;
  // unsigned long res = 0;
  // int           err;



// static void show_stmt_error(MYSQL_STMT *stmt)
// {
//   printf("Error(%d) [%s] \"%s\"\n", mysql_stmt_errno(stmt),
//                                   mysql_stmt_sqlstate(stmt),
//                                   mysql_stmt_error(stmt));
//   exit(-1);
// }


/* get handles  */
  conn = mysql_init(NULL);
  if (conn == NULL) {
    printf("couldn't initialize mysql: %s\n", mysql_error(conn));
    exit(1);
  }
  // stmt = mysql_stmt_init(conn);
  // if (stmt == NULL) {
  //   fprintf(stderr, "couldn't initialize statement: %s\n", mysql_error(conn));
  //   exit(0);
  // }

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




  if (mysql_query(conn, "SELECT * FROM Channels"))
    show_mysql_error(conn);
  
  // mysql_stmt_free_result(stmt);
  mysql_close(conn);
  printf("%s\n", "normal termination");
}
