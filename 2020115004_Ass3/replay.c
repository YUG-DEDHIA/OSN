#include "headers.h"

int check_replay(char **ar, int n, int *found, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd)
{
    int gt = 0;
    int interval;
    int period;
    if (strcmp(ar[0], "replay") == 0)
    {
        *found = 1;
        gt = 1;
        if (strcmp(ar[1], "-command") == 0)
        {
            char token[250];
            strcpy(token, "");
            int i = 2;
            while (strcmp(ar[i], "-interval") != 0)
            {
                int c = strlen(token);
                if (c != 0)
                {
                    token[c] = ' ';
                    token[c + 1] = '\0';
                }
                strcat(token, ar[i]);
                i++;
                if (i == n)
                {
                    printf("Invalid arguments\n");
                    return gt;
                }
            }
            if (i + 1 == n)
            {
                printf("Invalid arguments\n");
                return gt;
            }
            else
            {
                interval = ChangStrToInt(ar[i + 1]);
                if (i + 2 == n)
                {
                    printf("Invalid arguments\n");
                    return gt;
                }
                else if (strcmp(ar[i + 2], "-period") != 0)
                {
                    printf("Invalid arguments\n");
                    return gt;
                }
                else
                {
                    if (i + 4 != n)
                    {
                        printf("Invalid arguments\n");
                        return gt;
                    }
                    else
                    {
                        period = ChangStrToInt(ar[i + 3]);
                    }
                }
            }
            for (int i = interval; i <= period; i += interval)
            {
                sleep(interval);
                repeat(token, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, prev_stdin_fd, prev_stdout_fd);
                if (i + interval > period && i != period)
                {
                    sleep(period - i);
                }
            }
        }
        else
            printf("Invalid arguments\n");
    }
    return gt;
}