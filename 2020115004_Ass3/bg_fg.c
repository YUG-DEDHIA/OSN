#include "headers.h"

int check_bg_fg(char **ar, int c1, int *found)
{
    int gt = 0;
    if (strcmp("bg", ar[0]) == 0)
    {
        *found = 1;
        gt = 1;
        if (c1 == 2)
        {
            int job_count = ChangStrToInt(ar[1]);
            for (int i = 0; i < bg_count; i++)
            {
                if (bg_process[i].pid != 0 && bg_process[i].job_count == job_count)
                {
                    char *temp = getProcessStat(bg_process[i].pid);
                    if (strcmp(temp, "T") == 0)
                    {
                        kill(bg_process[i].pid, SIGCONT);
                    }
                    return gt;
                }
            }
            printf("No process with job number %d exists\n", job_count);
        }
        else
        {
            printf("Invalid number of arguments\n");
            return gt;
        }
    }
    else if (strcmp("fg", ar[0]) == 0)
    {
        *found = 1;
        gt = 1;
        if (c1 == 2)
        {
            int job_count = ChangStrToInt(ar[1]);
            for (int i = 0; i < bg_count; i++)
            {
                if (bg_process[i].pid != 0 && bg_process[i].job_count == job_count)
                {
                    char *temp = getProcessStat(bg_process[i].pid);
                    if (strcmp(temp, "T") == 0)
                    {
                        kill(bg_process[i].pid, SIGCONT);
                    }
                    waitpid(bg_process[i].pid, NULL, 0);
                    bg_process[i].pid = 0;
                    bg_process[i].job_count = -1;
                    active_process--;
                    return gt;
                }
            }
            printf("No process with job number %d exists\n", job_count);
        }
        else
        {
            printf("Invalid number of arguments\n");
            return gt;
        }
    }
    return gt;
}