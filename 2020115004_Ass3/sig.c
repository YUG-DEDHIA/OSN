#include "headers.h"

int check_sig(char **ar, int c, int *found)
{
    int gt=0;
    int sig,job_count;
    if(strcmp(ar[0],"sig")==0)
    {
        if(c==3)
        {
            *found=1;
            sig=ChangStrToInt(ar[2]);
            job_count=ChangStrToInt(ar[1]);
        }
        for(int i=0;i<bg_count;i++)
        {
            if(bg_process[i].pid!=0 && bg_process[i].job_count==job_count)
            {
                if(kill(bg_process[i].pid,sig)==-1)
                {
                    perror("kill");
                }
                
                gt=1;
                return gt;
            }
        }
        if(gt==0)
        {
            gt=1;
            printf("No process with job number %d exists\n",job_count);
        }
    }
    return gt;
}