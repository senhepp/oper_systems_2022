#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

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
    char* s = (char*) shmat(i, NULL, 0);
    for (;;) {
        time_t rawtime;
        struct tm* info;
        time(&rawtime);
        info = localtime(&rawtime);
        sprintf(s, "%d, %s", getpid(), asctime(info));
        sleep(3);
    }
    shmdt(s);
    shmctl(i, IPC_RMID, NULL);
    return 0;
}