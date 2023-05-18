#include "headers.h"
int CheckSystemProcess(char **args, int *found, int bg)
{
    int status;
    *found = 1;
    int forkRet = fork();
    if (forkRet == 0)
        {
            sleep(0.0001); // To prevent immediate termination of child process
            int stat = execvp(args[0], args);
            if(stat==-1)
            {
                return 0;
            }
            
        }
    if (bg == 0)
    {   
        strcpy(fg_process[fg_count].name,args[0]);
        fg_process[fg_count++].pid=forkRet;
        //waitpid(forkRet, &status, 0);
        while(waitpid(forkRet,&status,WNOHANG)==0 && x==0)
        {}
    }
    if(bg==1)
    {
        strcpy(bg_process[bg_count].name,args[0]);
        bg_process[bg_count].pid=forkRet;
        if(active_process==0)
        {
            job_count=1;
        }
        bg_process[bg_count].job_count=job_count;
        active_process++;
        bg_count++;
        job_count++;
        printf("%d\n",forkRet);

    }
    return 1;
    
}