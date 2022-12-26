#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv){
    int n = 0, s = 0, b = 0, E = 0;
    opterr = 0;
    int res = getopt(argc, argv, "nsbE");
    while (res != -1){
        switch (res){
            case 'n':
                n = 1;
                break;
            case 's':
                s = 1;
                break;
            case 'b':
                b = 1;
                break;
            case 'E':
                E = 1;
                break;
        }
        res = getopt(argc, argv, "nsbE");
    }

    FILE *fp;
    fp = fopen(argv[argc - 1], "r");
    if (fp){
        int lines = 1;
        char c = fgetc(fp), c1 = '\n';
        while (c != EOF){
            if (c == '\n' && c1 == '\n'){
                if (b || s){
                    if (s && !b){
                        int count = 0;
                        int l = lines;
                        while (l != 0){
                            l = l / 10;
                            count++;
                        }
                        for (int i = 0; i < 5 - count; i++){
                            printf(" ");
                        }
                        printf("%d ", lines);
                        lines++;
                    }
                    printf("\n");
                    c = fgetc(fp);
                    while (c == '\n'){
                        if (!s){
                            printf("\n");
                        }
                        c = fgetc(fp);
                    }
                }
            }

            if (c1 == '\n' && (n || b)){
                int count = 0;
                int l = lines;
                while (l != 0){
                    l = l / 10;
                    count++;
                }
                for (int i = 0; i < 5 - count; i++){
                    printf(" ");
                }
                printf("%d ", lines);
                lines++;
            }
            if (E && c == '\n'){
                printf("$");
            }
            printf("%c", c);
            c1 = c;
            c = fgetc(fp);
        }
        fclose(fp);
    }
    return 0;
}