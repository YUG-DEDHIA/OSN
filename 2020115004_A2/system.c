#include "headers.h"
int TokeizeWithSpace(char *token, char args[][100], int *bg)
{

    char temp4[200];
    int u2 = 0;
    strcpy(temp4, token);

    int n = strlen(temp4);

    int c1 = 0;
    int c2 = 0;
    if (temp4[n - 1] == ' ')
    {
        temp4[n - 1] = '\0';
        n--;
    }
    // for (int i = 0; i <= n; i++)
    int i = 0;
    while (i <= n)
    {
        if (temp4[i] == '&')
        {
            *bg = 1;
            i++;
        }
        if (temp4[i] == ' ')
        {
            args[c1++][c2] = '\0';
            c2 = 0;
        }
        if (temp4[i] == '\0')
        {
            args[c1++][c2] = '\0';
            c2 = 0;
        }
        else
            args[c1][c2++] = temp4[i];
        i++;
    }
    strcpy(args[c1], "\0");
    return c1;
}

int CheckSystemProcess(char **args, int *found, int bg)
{
    int status;
    int u1 = 0;
    *found = 1;
    int Foerkreturn = fork();
    if (Foerkreturn == 0)
    {
        sleep(0.001); // To prevent immediate termination of child process
        int stat = execvp(args[0], args);
        if (stat == -1)
        {
            return 0;
        }
    }
    if (bg == 0)
    {
        waitpid(Foerkreturn, &status, 0);
    }
    if (bg == 1)
    {
        strcpy(bg_process[bg_count].name, args[0]);
        bg_process[bg_count].pid = Foerkreturn;
        bg_count++;
        printf("%d\n", Foerkreturn);
    }
    return 1;
}