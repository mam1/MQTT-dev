
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



/* write an entry to the log files */
void logit(char *fname, char *source, char *mess) 
{
	// int 				size, msize;

	char 				*ptr;
	FILE 				*plog, *slog;
	char 				source_buff[25];
    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);

    int 				fill;

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

	memset(source_buff, ' ', sizeof(source_buff));
	source_buff[sizeof(source_buff)] = '\0';
	strcpy(source_buff, source);
	source_buff[strlen(source)] = ' ';

	fprintf(slog, "%s:\t%02d/%02d/%04d %02d:%02d:%02d - %s\n", source_buff, tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,tm.tm_hour, tm.tm_min, tm.tm_sec, mess);
	fclose(slog);

	return;
}