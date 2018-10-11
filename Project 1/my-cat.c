#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    for(int i=1;i<argc;i++){
        FILE *fp;
        int c;

        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("cannot open file\n");
            return 1;
        }

        while((c= fgetc(fp)) != EOF){
            putchar(c);
        }
        fclose(fp);
        if(i<argc){
            printf("\n");
            }
        }
    return 0;
}

