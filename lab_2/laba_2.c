#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void readFromPipe(int file)
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("PIPE:\nCHILD - %sPARENT - ", asctime(timeinfo));
    FILE *stream;
    int c;
    stream = fdopen(file, "r");
    while ((c = fgetc(stream)) != '\n') putchar(c);
    putchar('\n');
    fclose(stream);
}

void writeToPipe(int file)
{
    FILE *stream;
    stream = fdopen(file, "w");
    char str[10];
    sprintf(str, "%d", getpid());
    fprintf(stream, "%s", str);
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    fprintf(stream, " ");
    fprintf(stream, "%s", asctime(timeinfo));
    fclose(stream);
}

int main()
{
    pid_t pid;
    int mypipe[2];
    int fd_fifo;
    if (pipe(mypipe)) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }
    switch (pid = fork()) {
        case -1:
            fprintf(stderr, "Fork failed.\n");
            exit(1);
        case 0:
            sleep(5);
            readFromPipe(mypipe[0]);
            if ((fd_fifo = open("fifo", O_RDWR)) == -1) {
                fprintf(stderr, "Невозможно открыть fifo\n");
                exit(0);
            }
            char buf[250];
            memset(buf, 0, 250);
            if (read(fd_fifo, &buf, sizeof(buf)) == -1) {
                fprintf(stderr, "Невозможно прочесть из FIFO\n");
            }
            else {
                time_t rawtime;
                struct tm* timeinfo;
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                printf("FIFO:\nCHILD - %sPARENT - ", asctime(timeinfo));
                printf("%s", buf);
            }
            exit(0);
        default:
            writeToPipe(mypipe[1]);
            unlink("fifo");
            if ((mkfifo("fifo", 0666)) == -1) {
                fprintf(stderr, "Невозможно создать fifo\n");
                exit(0);
            }
            if ((fd_fifo = open("fifo", O_RDWR)) == -1) {
                fprintf(stderr, "Невозможно открыть fifo\n");
                exit(0);
            }
            char buffer[250];
            memset(buffer, 0, 250);
            time_t rawtime;
            struct tm* timeinfo;

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            sprintf(buffer, "%d %s", getpid(),  asctime(timeinfo));
            write(fd_fifo, buffer, strlen(buffer));
            wait(NULL);
    }
    return 0;
}