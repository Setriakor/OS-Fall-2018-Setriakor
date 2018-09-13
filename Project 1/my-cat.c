#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    FILE *fp;
    int c;

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf(argv[0] +": " + argv[1] + ": No such file or directory\n");
        return 1;
    }

    while((c= fgetc(fp)) != EOF){
        putchar(c);
    }
    fclose(fp);
    return 0;
}


