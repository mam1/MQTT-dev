/************************************************************************/
/*                                                                      */
/*  ipc.c - support for interprocess communication                          */
/*                                                                      */
/************************************************************************/

#include <errno.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>     //sleep,access
#include <stdint.h>     //uint_8, uint_16, uint_32, etc.
#include <sys/ipc.h>
#include <errno.h>

#include "/home/mam1/Git/MQTT-dev/include/typedefs.h"
#include "/home/mam1/Git/MQTT-dev/include/shared.h"
#include "/home/mam1/Git/MQTT-dev/include/ipc.h"

extern int              semid;
extern unsigned short   semval;
extern struct sembuf    sb;

union semun {
    int                 val;        // used for SETVAL only
    struct semid_ds     *buf;       // for IPC_STAT and IPC_SET
    uint8_t             *array;     // used for GETALL and SETALL
};

int ipc_open(char *fname, int size) {
    int                 fd;
    struct stat         sb;
    mode_t              mode = S_IRWXO | S_IRWXG | S_IRWXU;
    int                 result;

    fd = open(fname, O_RDWR | O_CREAT, mode);
    if (fd == -1)
        handle_error("open");
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");
    if (!S_ISREG (sb.st_mode)) {
        fprintf (stderr, "%s is not a file\n", fname);
        exit(1);
    }

    if (sb.st_size < size) {
        /* Stretch the file size to the size of the (mmapped) buffer */
        result = lseek(fd, size - 1, SEEK_SET);
        if (result == -1) {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            exit(EXIT_FAILURE);
        }

        result = write(fd, "", 1);
        if (result != 1) {
            close(fd);
            perror("Error writing last byte of the file");
            exit(EXIT_FAILURE);
        }
        if (fstat(fd, &sb) == -1)
            handle_error("fstat");
        if (!S_ISREG (sb.st_mode)) {
            fprintf (stderr, "%s is not a file\n", fname);
            exit(1);
        }
        close(fd);
        fd = open(fname, O_RDWR | O_CREAT, mode);
        if (fd == -1)
            handle_error("RDWR open");
        if (fstat(fd, &sb) == -1)
            handle_error("RDWR fstat");
        if (!S_ISREG (sb.st_mode)) {
            fprintf (stderr, "%s is not a file\n", fname);
            exit(1);
        }
        fd = open(fname, O_RDWR | O_CREAT, mode);
        if (fd == -1)
            handle_error("RDWR open");
        if (fstat(fd, &sb) == -1)
            handle_error("RDWR fstat");
        if (!S_ISREG (sb.st_mode)) {
            fprintf (stderr, "%s is not a file\n", fname);
            exit(1);
        }
    }

    return fd;
}

void *ipc_map(int fd, int size) {
    void        *data;
    struct stat         sb;

    data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
        handle_error("mmap");
    if (fstat(fd, &sb) == -1)
        handle_error("fstat2");
    if (!S_ISREG (sb.st_mode)) {
        fprintf (stderr, "is not a file2\n");
        exit(1);
    }
    return data;
}

void ipc_close(int fd, void *data, int size) {
    if (munmap(data, size) == -1)
        handle_error("munmap");

    close(fd);
    printf("  ipc_close: file unmapped\n");
    return;
}

int ipc_size(void) {

    long        page_size = sysconf (_SC_PAGESIZE);
    int         pages;
    // printf("    ipc data size %i\n",sizeof(IPC_DAT));
    pages = sizeof(_IPC_DAT) / page_size;
    // printf("    1 -pages = %i\n",pages);
    if (pages < 1) pages = 1;
    else if (sizeof(_IPC_DAT) % page_size != 0) pages += 1;
    // printf("    2 -pages = %i\n",pages);
//   printf("  ipc_size: system page size %i, ipc data %i, ipc data requires %i pages, %i bytes\n", (int)page_size, sizeof(IPC_DAT), pages, pages * (int)page_size);
    return (pages * (int)page_size);
}

int ipc_save(_IPC_DAT *ipc_ptr) {

    FILE        *ipc_bkup;
    // int      write_rtn;

    ipc_bkup = fopen(_IPC_FILE_BACKUP_NAME, "w");

    if (ipc_bkup == NULL) {                   // file not found
        perror(_IPC_FILE_BACKUP_NAME);
        return 1;
    }

    ipc_sem_lock(semid, &sb);                   // wait for a lock on shared memory

    if (fwrite(ipc_ptr, 1, sizeof(*ipc_ptr), ipc_bkup) != sizeof(*ipc_ptr)) {
        printf("\n*** error saving ipc backup data file\r\n");
        perror(_IPC_FILE_BACKUP_NAME);
        ipc_sem_free(semid, &sb);                   // free lock on shared memory
        fclose(ipc_bkup);

        return 1;
    }
    else
        ipc_sem_free(semid, &sb);                   // free lock on shared memory

    // #if defined (_ATRACE) || defined (_PTRACE)
    // trace(_TRACE_FILE_NAME, "\nipc", 0, NULL, "ipc data written to backup file\n", 0);
    // #endif
    fclose(ipc_bkup);
    return 0;
}

/* load shared memory from backup */
int ipc_load(_IPC_DAT *ipc_ptr) {

    int         rtn;
    FILE        *ipc_bkup;

    ipc_bkup = fopen(_IPC_FILE_BACKUP_NAME, "r+");
    if (ipc_bkup == NULL) {                    // file not found
        return 1;
    }
    ipc_sem_lock(semid, &sb);                   // wait for a lock on shared memory
    rtn = fread(ipc_ptr, sizeof(*ipc_ptr), 1, ipc_bkup);
    if (rtn != 1) {
        printf("\n*** error reading ipc backup data\n  fread returned %i\r\n", rtn);
        // perror(_TRACE_FILE_NAME);
        ipc_sem_free(semid, &sb);                   // free lock on shared memory
        return 1;
    }
    else
        ipc_sem_free(semid, &sb);                   // free lock on shared memory

    // #if defined (_ATRACE) || defined (_PTRACE)
    // trace(_TRACE_FILE_NAME, "\nipc", 0, NULL, "backup ipc file written to shared memory\n", 0);
    // #endif

    return 0;
}

int ipc_sem_init(void)
{
    key_t           key = _SEM_KEY;
    int             semid;
    union semun     arg;

    /* create a semaphore set with 1 semaphore: */
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }
    /* initialize semaphore #0 to 1: */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }
    return 0;
}

int ipc_sem_lock(int semid, struct sembuf *sb) {

    // printf(" ** %i locked\r\n",semid);
    sb->sem_num = 0;            // semaphore number
    sb->sem_op = -1;            // semaphore operation
    sb->sem_flg = 0;            // operation flags
    if (semop(semid, sb, 1) == -1) {
        perror("semop");
        exit(1);
    }

// #if defined (_ATRACE) || defined (_FTRACE)
//     trace(_TRACE_FILE_NAME, "\nipc", 0, NULL, "shared memory locked\n", 0);
// #endif

    return 0;
}

int ipc_sem_free(int semid, struct sembuf *sb) {

    sb->sem_op = 1; /* free resource */

    if (semop(semid, sb, 1) == -1) {
        perror("semop");
        exit(1);
    }

// #if defined (_ATRACE) || defined (_FTRACE)
//     trace(_TRACE_FILE_NAME, "\nipc", 0, NULL, "shared memory unlocked\n", 0);
// #endif

    return 0;
}

int ipc_sem_id(key_t skey) {
    int             semid;
    if ((semid = semget(skey, 1, 0)) == -1) {   //  grab the semaphore set
        perror("semget");
        exit(1);
    }
    return semid;
}