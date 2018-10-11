#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *fp;
    char line[9999];

    if(argc == 1){
        printf("my-grep: searchterm [file ...]\n");
        return 1;
    }
    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            fp = fopen(argv[i], "r");
            if (fp == NULL)
            {
                printf("my-grep: cannot open file\n");
                return 1;
            }
            while (fgets(line, sizeof line, fp) != NULL)
            {
                if (strstr(line, argv[1]) != NULL)
                {
                    printf(line);
                }
            }
            fclose(fp);
        }
    }else{
        while (fgets(line, sizeof line, stdin))
            {
                if (strstr(line, argv[1]) != NULL)
                {
                    printf(line);
                }
            }
    }

    return 0;
}
