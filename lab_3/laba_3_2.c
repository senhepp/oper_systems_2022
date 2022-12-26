#include <stdio.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    key_t k = ftok("./a", 0);
    int g = shmget(k, 1000, 0);
    if (g == -1) {
        return EXIT_FAILURE;
    }
    char* s = (char*) shmat(g, NULL, 0);
    time_t rawtime;
    struct tm* info;
    time(&rawtime);
    info = localtime(&rawtime);
    printf("%d, %s%s", getpid(), asctime(info), s);
    return 0;
}