#include "headers.h"
void child_handler(int sig)
{
    int status;
    for (int i = 0; i < bg_count; i++)
    {
        // printf("\n\n%d\n\n",p_arr[i]);
        if (waitpid(bg_process[i].pid, &status, WNOHANG) != 0 && bg_process[i].pid != 0)
        {
            if (WIFEXITED(status))
                printf("\n%s with pid %d has ended normally\n", bg_process[i].name, bg_process[i].pid);
            else
                printf("\n%s with pid %d has ended abnormally\n", bg_process[i].name, bg_process[i].pid);
            bg_process[i].pid = 0;
            bg_process[i].job_count = -1;
            active_process--;
        }
    }
    return;
}
void CtrlC_handler(int sig)
{
    signal(SIGINT, CtrlC_handler);
    if (waitpid(fg_process[fg_count - 1].pid, NULL, WNOHANG) == 0)
    {
        if (kill(fg_process[fg_count - 1].pid, SIGINT) == -1)
        {
            perror("Ctrl-C kill");
        }
    }
}
void CtrlZ_handler(int sig)
{
    signal(SIGTSTP, CtrlZ_handler);
    if (waitpid(fg_process[fg_count - 1].pid, NULL, WNOHANG) == 0 && strcmp(getProcessStat(fg_process[fg_count - 1].pid),"T")!=0)
    {
        x=1;
        if (kill(fg_process[fg_count - 1].pid, SIGSTOP) == -1)
        {
            perror("Ctrl-Z kill");
        }
        strcpy(bg_process[bg_count].name,fg_process[fg_count-1].name);
        bg_process[bg_count].pid=fg_process[fg_count-1].pid;
        if(active_process==0)
        {
            job_count=1;
        }
        bg_process[bg_count].job_count=job_count;
        active_process++;
        bg_count++;
        job_count++;
    }
}
