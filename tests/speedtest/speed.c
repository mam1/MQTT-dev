#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define _QUOTE      34

int main(void)
{

	char 			*speedtest = "speedtest --format=csv > /home/mam1/temp_speed.csv";
	char 			*filename = "/home/mam1/temp_speed.csv";
	char 			value[12][100];
	char 			timebuff[100], *ptr;
	int 			i;
	int 			qon;


	FILE 			*fp;
	long 			lSize;
	char 			*buffer, *bptr, *vptr;
	int 			fnum = 0;

	time_t rawtime;
	struct tm * timeinfo;

	char *host = "beaglebone";

	memset(value, '\0', sizeof(value));

	/* get local time */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(timebuff, "%s:%s:%s\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	strcpy (&value[fnum++][0], timebuff);

	/* set host */
	strcpy (&value[fnum++][0], host);

	/* run speedtest */
	system(speedtest);

	/* process result */
	fp = fopen (filename, "rb" );
	if ( !fp )
	{
		printf("%s\n", "error on file
		       exit (1);
	       }

		       fseek( fp , 0L , SEEK_END);
		       lSize = ftell( fp );
		       rewind( fp );

		       /* allocate memory for entire content */
		       buffer = calloc( 1, lSize + 1 );
		       if ( !buffer ) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

		       /* copy the file into the buffer */
		       if ( 1 != fread( buffer , lSize, 1 , fp) )
		       fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

		       /* do your work here, buffer is a string contains the whole text */

		       bptr = buffer;



		       while (*bptr != '\0')
	       {
		       if (*bptr == _QUOTE)
	       {
		       qon = 1;
		       vptr = &value[fnum++][0];
		       bptr++;
	       }
		       while ((qon == 1) & (*bptr != '\0'))
	       {
		       if (*bptr == _QUOTE)
	       {
		       qon = 0;
		       bptr++;
	       }
		       else
		       *vptr++ = *bptr++;
		       *vptr = '\0';
	       }

		       bptr++;
	       }

		       printf("buffer < % s > \n", buffer);

		       fclose(fp);
		       free(buffer);


		       for (i = 0; i < 12; i++)
	       {
		       printf("field % i value < % s > \n", i, &value[i][0]);
	       }

		       printf(" % s\n", "normal termination");
		       return (0);
	       }

