/* Copyright (C) 2005 - 2019 Hartmut Holzgraefe <hartmut@php.net>
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * MySQL C client API example: mysql_stmt_bind_result()
 *
 * see also http://mysql.com/mysql_stmt_bind_result
 */

// #include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "/usr/include/mariadb/mysql.h"

// #include <mysql.h>

#define STRING_SIZE 100

// #define SELECT_SAMPLE "SELECT tokenID, token, type, value FROM TokenQ ORDER BY tokenID LIMIT 1"
#define SELECT_SAMPLE "SELECT tokenID, token, type, value FROM TokenQ WHERE !SLEEP(1)"

int main(int argc, char **argv) {
  MYSQL_STMT    *stmt;
  MYSQL_BIND    bind[4];
  MYSQL_RES     *prepare_meta_result;
  MYSQL_TIME    ts;
  unsigned long length[4];
  int           param_count, column_count, row_count;
  short         small_data;
  int           int_data_1,int_data_2;
  char          str_data_1[STRING_SIZE];
    char          str_data_2[STRING_SIZE];
  my_bool       is_null[4];
  my_bool       error[4];
  MYSQL *mysql = NULL;
  int j;
  my_bool reconnect = 1;

  if (mysql_library_init(argc, argv, NULL)) {
    fprintf(stderr, "could not initialize MySQL client library\n");
    exit(1);
  }

  mysql = mysql_init(mysql);

  if (!mysql) {
    puts("Init faild, out of memory?");
    return EXIT_FAILURE;
  }

  mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);

  mysql_options(mysql, MYSQL_READ_DEFAULT_FILE, (void *)"./test.cnf");

  if (!mysql_real_connect(mysql,       /* MYSQL structure to use */
                          NULL,         /* server hostname or IP address */
                          NULL,         /* mysql user */
                          NULL,          /* password */
                          NULL,           /* default database to use, NULL for none */
                          0,           /* port number, 0 for default */
                          NULL,        /* socket file or named pipe name */
                          CLIENT_FOUND_ROWS /* connection flags */ )) {
    puts("Connect failed\n");
    return EXIT_FAILURE;
  }

  printf("connection id: %ld\n", mysql_thread_id(mysql));

  /* Prepare a SELECT query to fetch data from test_table */
  stmt = mysql_stmt_init(mysql);
  if (!stmt)
  {
    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
    exit(0);
  }
  if (mysql_stmt_prepare(stmt, SELECT_SAMPLE, strlen(SELECT_SAMPLE)))
  {
    fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    exit(0);
  }
  fprintf(stdout, " prepare, SELECT successful\n");

  /* Get the parameter count from the statement */
  param_count = mysql_stmt_param_count(stmt);
  fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

  if (param_count != 0) /* validate parameter count */
  {
    fprintf(stderr, " invalid parameter count returned by MySQL\n");
    exit(0);
  }

  /* Fetch result set meta information */
  prepare_meta_result = mysql_stmt_result_metadata(stmt);
  if (!prepare_meta_result)
  {
    fprintf(stderr,
            " mysql_stmt_result_metadata(), returned no meta information\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    exit(0);
  }

  /* Get total columns in the query int_data*/
  column_count = mysql_num_fields(prepare_meta_result);
  fprintf(stdout, " total columns in SELECT statement: %d\n", column_count);

  if (column_count != 4) /* validate column count */
  {
    fprintf(stderr, " invalid column count returned by MySQL\n");
    exit(0);
  }

  /* Bind the result buffers for all 4 columns before fetching them */

  memset(bind, 0, sizeof(bind));

  /* INTEGER COLUMN */
  bind[0].buffer_type = MYSQL_TYPE_LONG;
  bind[0].buffer = (char *)&int_data_1;
  bind[0].is_null = &is_null[0];
  bind[0].length = &length[0];
  bind[0].error = &error[0];

  /* STRING COLUMN */
  bind[1].buffer_type = MYSQL_TYPE_STRING;
  bind[1].buffer = (char *)str_data_1;
  bind[1].buffer_length = STRING_SIZE;
  bind[1].is_null = &is_null[1];
  bind[1].length = &length[1];
  bind[1].error = &error[1];

  /* STRING COLUMN */
  bind[2].buffer_type = MYSQL_TYPE_STRING;
  bind[2].buffer = (char *)str_data_2;
  bind[2].buffer_length = STRING_SIZE;
  bind[2].is_null = &is_null[2];
  bind[2].length = &length[2];
  bind[2].error = &error[2];

  /* INTEGER COLUMN */
  bind[3].buffer_type = MYSQL_TYPE_LONG;
  bind[3].buffer = (char *)&int_data_2;
  bind[3].is_null = &is_null[3];
  bind[3].length = &length[3];
  bind[3].error = &error[3];

  /* Bind the result buffers */
  if (mysql_stmt_bind_result(stmt, bind))
  {
    fprintf(stderr, " mysql_stmt_bind_result() failed\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    exit(0);
  }


  // for (j = 0; j < 3; j++) {
    /* Execute the SELECT query */
    if (mysql_stmt_execute(stmt))
    {
      fprintf(stderr, " mysql_stmt_execute(), failed\n");
      fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    } else {

      /* Now buffer all results to client */
      if (mysql_stmt_store_result(stmt))
      {
        fprintf(stderr, " mysql_stmt_store_result() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(0);
      }

      /* Fetch all rows */
      row_count = 0;
      fprintf(stdout, "Fetching results ...\n");
      while (!mysql_stmt_fetch(stmt))
      {
        row_count++;
        fprintf(stdout, "  row %d\n", row_count);

        /* column 1 */
        fprintf(stdout, "   column1 (integer)  : ");
        if (is_null[0])
          fprintf(stdout, " NULL\n");
        else
          fprintf(stdout, " %d(%ld)\n", int_data_1, length[0]);

        /* column 2 */
        fprintf(stdout, "   column2 (string)   : ");
        if (is_null[1])
          fprintf(stdout, " NULL\n");
        else
          fprintf(stdout, " %s(%ld)\n", str_data_1, length[1]);

        /* column 3 */
        fprintf(stdout, "   column2 (string)   : ");
        if (is_null[2])
          fprintf(stdout, " NULL\n");
        else
          fprintf(stdout, " %s(%ld)\n", str_data_2, length[2]);

        /* column 4 */
        fprintf(stdout, "   column1 (integer)  : ");
        if (is_null[3])
          fprintf(stdout, " NULL\n");
        else
          fprintf(stdout, " %d(%ld)\n", int_data_2, length[3]);
        fprintf(stdout, "\n");
      }

      /* Validate rows fetched */
      fprintf(stdout, " total rows fetched: %d\n", row_count);
      if (row_count != 2)
      {
        fprintf(stderr, " MySQL failed to return all rows\n");
        exit(0);
      }
    }
  // }

  /* Free the prepared result metadata */
  puts("freeing metadata\n");
  mysql_free_result(prepare_meta_result);

  /* Close the statement */
  puts("closing statement\n");
  if (mysql_stmt_close(stmt))
  {
    fprintf(stderr, " failed while closing the statement\n");
    fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
    exit(0);
  }

  printf("connection id: %ld\n", mysql_thread_id(mysql));
  puts("closing connection\n");
  mysql_close(mysql);

  mysql_library_end();

  return EXIT_SUCCESS;
}






// #include "mysql.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// static void show_mysql_error(MYSQL *mysql)
// {
//   printf("Error(%d) [%s] \"%s\"\n", mysql_errno(mysql),
//                                   mysql_sqlstate(mysql),
//                                   mysql_error(mysql));
//   exit(-1);
// }

// static void show_stmt_error(MYSQL_STMT *stmt)
// {
//   printf("Error(%d) [%s] \"%s\"\n", mysql_stmt_errno(stmt),
//                                   mysql_stmt_sqlstate(stmt),
//                                   mysql_stmt_error(stmt));
//   exit(-1);
// }

// int main(int argc, char *argv[])
// {
//   MYSQL *mysql;
//   MYSQL_STMT *stmt;
//   MYSQL_BIND bind[3];

//   /* Data for insert */
//   const char *surnames[]= {"Widenius", "Axmark", "N.N."};
//   unsigned long surnames_length[]= {8,6,4};
//   const char *forenames[]= {"Monty", "David", "will be replaced by default value"};
//   char forename_ind[]= {STMT_INDICATOR_NTS, STMT_INDICATOR_NTS, STMT_INDICATOR_DEFAULT};
//   char id_ind[]= {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};
//   unsigned int array_size= 3;

//   mysql= mysql_init(NULL);

//   /* connect to MariaDB server */
//   if (!mysql_real_connect(mysql, "localhost", "test-sql", "test-sql",
//                           "example", 0, "/run/mysqld/mysqld.sock", 0))
//     show_mysql_error(mysql);

//   printf("connection established ...\n");

//   if (mysql_query(mysql, "DROP TABLE IF EXISTS bulk_example1"))
//     show_mysql_error(mysql);

//   if (mysql_query(mysql, "CREATE TABLE bulk_example1 (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"\
//                          "forename CHAR(30) NOT NULL DEFAULT 'unknown', surname CHAR(30))"))
//     show_mysql_error(mysql);

//   stmt= mysql_stmt_init(mysql);
//   if (mysql_stmt_prepare(stmt, "INSERT INTO bulk_example1 VALUES (?,?,?)", -1))
//     show_stmt_error(stmt);

//   memset(bind, 0, sizeof(MYSQL_BIND) * 3);

//   /* We autogenerate id's, so all indicators are STMT_INDICATOR_NULL */
//   bind[0].u.indicator= id_ind;
//   bind[0].buffer_type= MYSQL_TYPE_LONG;

//   bind[1].buffer= forenames;
//   bind[1].buffer_type= MYSQL_TYPE_STRING;
//   bind[1].u.indicator= forename_ind;

//   bind[2].buffer_type= MYSQL_TYPE_STRING;
//   bind[2].buffer= surnames;
//   bind[2].length= &surnames_length;

//   /* set array size */
//   mysql_stmt_attr_set(stmt, STMT_ATTR_ARRAY_SIZE, &array_size);

//   /* bind parameter */
//   mysql_stmt_bind_param(stmt, bind);

//   /* execute */
//   if (mysql_stmt_execute(stmt))
//     show_stmt_error(stmt);

//   mysql_stmt_close(stmt);
//   mysql_close(mysql);

//   printf("%s\n", "made it this far!!!!\n");
// }