
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <ctype.h> 		//isalnum, tolower
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"



/* write an entry to the  log files */
void logit(char *fname, char *source, char *mess) 
{
	FILE 				*plog, *slog;


    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);




	// char 				time_now[]="0:0:0";
	// char  				*tnptr;
	// time_t 				t;

	// t = time(NULL);
	// time_now = ctime(&t);
// 	tnptr = time_now;
// 	while (*tnptr != _CR) tnptr++;
// 	*tnptr = '\0';

// printf("%s\n", "got the time");
printf("fname <%s>,  source <%s>,  mess <%s>\n", fname, source, mess );
     
    printf("System Date is: %02d/%02d/%04d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    printf("System Time is: %02d:%02d:%02d\n",tm.tm_hour, tm.tm_min, tm.tm_sec);


// return;

	/* Open process log file */
	plog = fopen(fname, "a");
	if (plog == NULL) {
		exit(EXIT_FAILURE);
	}

	fprintf(plog, "%02d/%02d/%04d %02d:%02d:%02d - %s\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec, mess);
	fclose(plog);


	/* Open system log file */
	slog = fopen(_SYSTEM_LOG, "a");
	if (slog == NULL) {
		exit(EXIT_FAILURE);
	}
	fprintf(slog, "%s: %02d/%02d/%04d %02d:%02d:%02d - %s\n", source, tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec, mess);
	fclose(slog);



	return;
}