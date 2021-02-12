// in modern C programs, you can simplify this another way, by putting the console I/O into a thread or lightweight process.
// Then the I/O can go on in the usual blocking fashion, but the data can be inserted into a queue to be processed on another thread.

#include <unistd.h>   //sleep
#include <stdio.h>
#include "ipc.h"

int main(int argc, char *argv[]) {

  char              file[] = "conwatcher";
  pid_t             fork_pid;
  char              *args[] = {file, NULL};

  key_t             skey = _SEM_KEY;
  int               semid;

  /* global memory mapped io variables */
  unsigned short    semval;
  union semun {
    int val;                            // used for SETVAL only
    struct semid_ds *buf;               // for IPC_STAT and IPC_SET
    ushort *array;                      // used for GETALL and SETALL
  };
  union       semun dummy;
  SEMBUF sb = {0, -1, 0};               // set to allocate resource

/* setup shared memory */
  ipc_sem_init();
  semid = ipc_sem_id(skey);             // get semaphore id
  ipc_sem_lock(semid, &sb);             // wait for a lock on shared memory

  fd = ipc_open(ipc_file, ipc_size());  // create/open ipc file
  data = ipc_map(fd, ipc_size());       // map file to memory
  ipc_ptr = (_IPC_DAT *)data;           // overlay ipc data structure on shared memory
  ipc_sem_free(semid, &sb);             // free lock on shared memory

  if(ipc==0){
    printf("* new ipc file created and initialized" );



    // ipc_sem_lock(semid, &sb);                   // wait for a lock on shared memory
    //     // ipc_ptr->sys_data.config.major_version = _MAJOR_VERSION_system;
    //     // ipc_ptr->sys_data.config.minor_version = _MINOR_VERSION_system;
    //     // ipc_ptr->sys_data.config.minor_revision = _MINOR_REVISION_system;
    //     // ipc_ptr->sys_data.config.channels = _NUMBER_OF_CHANNELS;
    //     // ipc_ptr->sys_data.config.sensors = _NUMBER_OF_SENSORS;
    //     // ipc_ptr->sys_data.config.commands = _CMD_TOKENS;
    //     // ipc_ptr->sys_data.config.states = _CMD_STATES;
    //     ipc_sem_free(semid, &sb);                   // free lock on shared memory
  }






  fork_pid = fork();

  if (fork_pid == 0)
  {
    printf("Hello from the child!\n");
    execvp(args[0], args);
  }

  printf("Hello from the parent!\n");
  printf("%s\n", "main process running");
  sleep(10);
  printf("%s\n", "main process running");
  sleep(100);
  printf("%s\n", "main process running");


  return 0;
}

// #include <unistd.h>
// int main(int argc, char *argv[]) {
//   execl("/bin/bash", "/yes/its/bash", "-c", "echo $0 && uptime", NULL);

//   return 0;
// }