#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

void prt(int batch, char *prompt)
{
    if (batch != 1)
    {
        printf(prompt);
    }
}
void run(void *tArgs)
{
    char **args;
    args = (char **)tArgs;
    int child_pid = fork();
    // printf(args[0]);
    // printf(args[1]);
    // usleep(5000000);
    if (child_pid == 0)
    {
        /* This is done by the child process. */
        execvp(args[0], args);

        /* If execv returns, it must have failed. */

        printf("Unknown command\n");
        exit(0);
    }
    else
    {
        /* This is run by the parent.  Wait for the child
        to terminate. */
        int wc = wait(NULL);
    }
}

void exe(char *inst, const char *argArr[], char *prompt, int mode, int argSize)
{
    int child_pid = fork();
    if (child_pid == 0)
    {
        /* This is done by the child process. */
        if (strcmp(argArr[argSize - 3], ">") == 0)
        {
            int fp = open(argArr[argSize - 2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            dup2(fp, 1);
            argArr[argSize - 2] = NULL;
            argArr[argSize - 3] = NULL;
        }
        execvp(inst, argArr);

        /* If execv returns, it must have failed. */

        printf("Unknown command\n");
        exit(0);
    }
    else
    {
        /* This is run by the parent.  Wait for the child
        to terminate. */
        int wc = wait(NULL);
        prt(mode, prompt);
    }
}

void mExe(char *allArgs[], int numComs, int size, char *prompt, int mode)
{
    pthread_t threads[numComs];
    int rc;
    int t = 0;
    for (int i = 0; i < size; i++)
    {
        char *command = allArgs[i];
        int numArgs = 1;
        for (int k = i + 1; k < size; k++)
        {
            if (strcmp(allArgs[k], "&") == 0)
            {
                break;
            }
            numArgs++;
        }
        if (numArgs == 1)
        {
            char *miniArgs = {NULL};
            rc = pthread_create(&threads[t], NULL, run, (void *)miniArgs);
            t++;
        }
        else
        {
            char *miniArgs[numArgs + 1];
            miniArgs[0] = command;
            for (int j = 1; j < numArgs + 1; j++)
            {
                if (j == numArgs)
                {
                    if (i == size - 1)
                    {
                        miniArgs[strlen(allArgs[i + 1]) - 1] = 0;
                        miniArgs[j] = allArgs[i + 1];
                    }
                    //printf(command);
                    //printf("\n");
                }
                else
                {
                    miniArgs[j] = allArgs[i + 1];
                    //printf(miniArgs[j]);
                    //printf("\n");
                }
            }
            miniArgs[numArgs] = 0;
            rc = pthread_create(&threads[t], NULL, run, (void *)miniArgs);
            t++;
        }
        i += numArgs;
    }
    prt(1,prompt);
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    char *buffer;
    char *buffer1;
    char *arg1;
    char *arg2;
    char *prompt;
    char *path;
    char *paths;
    char *root;
    char *com;
    char *copy;
    int mode;
    size_t bufsize = 32;

    buffer = (char *)malloc(bufsize * sizeof(char));
    prompt = (char *)malloc(bufsize * sizeof(char));
    path = (char *)malloc(bufsize * sizeof(char));
    paths = (char *)malloc(bufsize * sizeof(char));
    arg2 = (char *)malloc(bufsize * sizeof(char));
    com = (char *)malloc(bufsize * sizeof(char));
    root = (char *)malloc(bufsize * sizeof(char));
    copy = (char *)malloc(bufsize * sizeof(char));

    strcpy(path, "/bin");
    prompt = "wish> ";
    if (argc == 1)
    {
        fp = stdin;
        printf(prompt);
        mode = 0;
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            printf("cannot open file\n");
            return 1;
        }
        mode = 1;
    }
    else
    {
        return 1;
    }
    while (getline(&buffer, &bufsize, fp) != -1)
    {
        buffer1 = buffer;
        arg1 = strtok_r(buffer1, " ", &buffer1);

        if (strcmp(arg1, "exit\n") == 0)
        {
            exit(0);
        }
        else if (strcmp(arg1, "cd") == 0)
        {
            arg2 = strtok_r(NULL, " ", &buffer1);
            char *arg3 = strtok_r(NULL, " ", &buffer1);
            if (arg2 == NULL)
            {
                printf("argument not found\n");
            }
            else if (arg3 != NULL)
            {
                printf("too many arguments\n");
            }
            else
            {
                arg2[strlen(arg2) - 1] = 0;
                if (chdir(arg2) == 0)
                {
                    printf("changed path\n");
                }
                else
                {
                    printf("unable to find path\n");
                }
            }
            prt(mode, prompt);
        }
        else if (strcmp(arg1, "path") == 0)
        {
            buffer1[strlen(buffer1) - 1] = 0;
            strcpy(path, buffer1);
            printf(path);
            printf("\n");
            prt(mode, prompt);
        }
        else
        {
            int reg = 1;
            strcpy(paths, path);
            arg2 = strtok_r(paths, " ", &paths);
            if (arg1[strlen(arg1) - 1] == '\n')
            {
                arg1[strlen(arg1) - 1] = 0;
                while (arg2 != NULL)
                {
                    strcpy(com, arg2);
                    strcat(com, "/");
                    strcat(com, arg1);
                    if (access(com, X_OK) == 0)
                    {
                        reg = 0;
                        break;
                    }
                    else
                    {
                        arg2 = strtok_r(NULL, " ", &paths);
                    }
                }
                if (reg == 1)
                {
                    //strcpy(com, "unknown");
                }
                const char *args = {NULL};
                exe(com, args, prompt, mode, 1);
            }
            else
            {
                int multi = 1;
                while (arg2 != NULL)
                {
                    strcpy(com, arg2);
                    strcat(com, "/");
                    strcat(com, arg1);
                    if (access(com, X_OK) == 0)
                    {
                        reg = 0;
                        break;
                    }
                    else
                    {
                        arg2 = strtok_r(NULL, " ", &paths);
                    }
                }
                if (reg == 1)
                {
                    //strcpy(com, "unknown");
                }
                strcpy(copy, buffer1);
                int i = 2;
                char *count = strtok_r(copy, " ", &copy);
                while (count != NULL)
                {
                    i++;
                    count = strtok_r(NULL, " ", &copy);
                }
                char *args[i];
                args[0] = arg1;
                for (int k = 1; k < i - 1; k++)
                {
                    arg2 = strtok_r(NULL, " ", &buffer1);
                    if (strcmp(arg2, "&") == 0)
                    {
                        multi++;
                    }
                    args[k] = arg2;
                    if (k == (i - 2))
                    {
                        arg2[strlen(arg2) - 1] = 0;
                        args[k] = arg2;
                    }
                }
                args[i - 1] = 0;
                if (multi == 1)
                {
                    exe(com, args, prompt, mode, i);
                }
                else
                {
                    mExe(args, multi, i - 1, prompt, mode);
                }
            }
        }
    }
    return 0;
}