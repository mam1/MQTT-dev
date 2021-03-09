#ifndef IPC_H_
#define IPC_H_

#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"

/* ipc version info */
#define _MAJOR_VERSION_ipc    10
#define _MINOR_VERSION_ipc   4

/* ipc fuctions */
 int 	ipc_open(char *,int);
 void 	*ipc_map(int, int);
 void 	ipc_close(int, void *, int);
 int 	ipc_size(void);
 int 	ipc_sem_init(void);
 int 	ipc_sem_lock(int semid, SEMBUF *sb);
 int 	ipc_sem_free(int semid, SEMBUF *sb);
 int 	ipc_sem_id(int skey);
 int 	ipc_save(_IPC_DAT *ipc_ptr);
 int 	ipc_load(_IPC_DAT *ipc_ptr);

#endif