
#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //sleep
#include <string.h>

/* in the sql table inode is an int unsigned and rpath is a varchar */
#define SAMPLE_QUERY "select * from channels"

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
