
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



/* write an entry to the daemon log file */
void logit(char *fname, char *source, char *mess) {

printf("%s\n", "hi there");
return;

	FILE 				*dlog, *slog;
	char 				* time_now, *tnptr;
	time_t 				t;

	t = time(NULL);
	time_now = ctime(&t);
	tnptr = time_now;
	while (*tnptr != _CR) tnptr++;
	*tnptr = '\0';

printf("%s\n", "got the time");
printf("fname <%s>,  source <%s>,  mess <%s>\n", fname, source, mess );

return;

	/* Open process log file */
	dlog = fopen(fname, "a");
	if (dlog == NULL) {
		exit(EXIT_FAILURE);
	}

	fprintf(dlog, "%s - %s\n", time_now, mess);
	fclose(dlog);

return;

	/* Open system log file */
	slog = fopen(_SYSTEM_LOG, "a");
	if (slog == NULL) {
		exit(EXIT_FAILURE);
	}

	fprintf(slog, "%s: %s - %s\n",source, time_now, mess);
	fclose(slog);



	return;
}