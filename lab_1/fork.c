#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sigtermSignal(int s) {
    if (s == SIGTERM) {
        printf("SIGTERM\n");
    }
}

void sigintSignal()
{
    printf("SIGINT\n");
}

void f1()
{
    printf("F1\n");
}

int main()
{
    pid_t pid;
    switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printf(" CHILD: ЭТО ПРОЦЕСС-ПОТОМОК!\n");
            printf(" CHILD: МОЙ PID -- %d\n", getpid());
            printf(" CHILD: PID МОЕГО РОДИТЕЛЯ -- %d\n", getppid());
            printf(" CHILD: ВЫХОД!\n");
            return 0;
        default:
            printf(" PARENT: ЭТО ПРОЦЕСС-РОДИТЕЛЬ!\n");
            printf(" PARENT: МОЙ PID -- %d\n", getpid());
            printf(" PARENT: PID МОЕГО ПОТОМКА -- %d\n", pid);
    }
    if (atexit(f1)) {
        return -1;
    }
    signal(SIGINT, sigintSignal);
    struct sigaction s;
    s.sa_handler = sigtermSignal;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGTERM, &s, 0);
    int a = 1;
    while (a) {
        scanf("%d", &a);
    }
    return 0;
}