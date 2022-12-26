#include <stdio.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>

int main()
{
    key_t k = ftok("./a", 0);
    int g = shmget(k, 1000, 0);
    if (g == -1) {
        return EXIT_FAILURE;
    }
    int sem = semget(k, 1, 0);
    if (sem == -1) {
        return EXIT_FAILURE;
    }
    struct sembuf b;
    b.sem_num = 0;
    b.sem_flg = 0;
    b.sem_op = 0;
    semop(sem, &b, 1);
    b.sem_op = 1;
    semop(sem, &b, 1);
    char* s = (char*) shmat(g, NULL, 0);
    time_t rawtime;
    struct tm* info;
    time(&rawtime);
    info = localtime(&rawtime);
    printf("%d, %s%s", getpid(), asctime(info), s);
    b.sem_op = -1;
    semop(sem, &b, 1);
    return 0;
}