#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>

int main()
{
    key_t k = ftok("./a", 0);
    int i = shmget(k, 1000, IPC_CREAT | 0666 | IPC_EXCL);
    if (i == -1) {
        if (errno == EEXIST) {
            perror("EEXIST");
            return EXIT_FAILURE;
        }
    }
    int sem = semget(k, 1, IPC_CREAT | 0666 | IPC_EXCL);
    if (sem == -1) {
        return EXIT_FAILURE;
    }
    char* s = (char*) shmat(i, NULL, 0);
    for (;;) {
        struct sembuf b;
        b.sem_num = 0;
        b.sem_flg = 0;
        b.sem_op = 0;
        semop(sem, &b, 1);
        b.sem_op = 1;
        semop(sem, &b, 1);
        time_t rawtime;
        struct tm* info;
        time(&rawtime);
        info = localtime(&rawtime);
        sprintf(s, "%d, %s", getpid(), asctime(info));
        b.sem_op = -1;
        semop(sem, &b, 1);
        sleep(3);
    }
    shmdt(s);
    shmctl(i, IPC_RMID, NULL);
    semctl(sem, IPC_RMID, 0);
    return 0;
}