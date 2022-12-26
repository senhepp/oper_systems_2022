#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    opterr = 0;
    char* name;
    int f = 0;
    int res = getopt(argc, argv, "f:");
    while (res != -1) {
        switch (res) {
            case 'f':
                f = 1;
                name = optarg;
                break;
        }
        res = getopt(argc, argv, "f:");
    }
    if (f == 1) {
        FILE* file = fopen(name, "r");
        if (file) {
            char *line = NULL;
            size_t len = 0;
            ssize_t nread;
            while ((nread = getline(&line, &len, file)) != -1) {
                if (strstr(line, argv[argc - 1]) != NULL) {
                    printf("%s", line);
                }
            }
        }
    } else {
        char *line = NULL;
        size_t len = 0;
        ssize_t nread;
        while ((nread = getline(&line, &len, stdin)) != -1) {
            if (strstr(line, argv[argc - 1]) != NULL) {
                printf("%s", line);
            }
        }
    }
	return 0;
}