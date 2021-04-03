#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

int main(void)
{

	char 			*speedtest = "speedtest --format=csv > /home/mam1/temp_speed";
	FILE 			*tempfile;
	char 			*filename = "/home/mam1/temp_speed.csv";
	char 			value[10][50];
	int 			i;

	/* run speedtest */
	system(speedtest);
	printf("%s\n", "speed test comleted");

	/* parse string return from speetest.net  */

FILE *fp;
long lSize;
char *buffer;

fp = fopen ( "filename" , "rb" );
if( !fp ) pinerror("blah.txt"),exit(1);

fseek( fp , 0L , SEEK_END);
lSize = ftell( fp );
rewind( fp );

/* allocate memory for entire content */
buffer = calloc( 1, lSize+1 );
if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

/* copy the file into the buffer */
if( 1!=fread( buffer , lSize, 1 , fp) )
  fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

/* do your work here, buffer is a string contains the whole text */

printf("buffer <%s>\n", buffer);

fclose(fp);
free(buffer);














	// tempfile = fopen(filename, "r");
	// if (tempfile == NULL)
	// {
	// 	printf("Error!");
	// 	exit(1);
	// }
	// rewind(tempfile);
	// fscanf(tempfile, "%s, %s, %s, %s, %s, %s, %s, %s, %s, %s", value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7], value[8], value[9]);
	// fclose(tempfile);

	for (i = 0; i < 10; i++)
	{
		printf("field %i value <%s>\n", i, &value[i][0]);
	}

	printf("%s\n", "abnormal termination");
	return (0);
}

