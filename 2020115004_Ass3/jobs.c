#include "headers.h"

int check_jobs(char **ar, int c1, int *found)
{
    int gt = 0;
    if (strcmp(ar[0], "jobs") == 0)
    {
        if (c1 > 3)
        {
            *found = 0;
            gt = 1;
            return gt;
        }
        *found = 1;
        if (c1 > 1)
        {
            if (strcmp(ar[1], "-r")==0)
            {
                if (c1 > 2)
                {
                    if (strcmp(ar[2], "-s") == 0)
                    {
                        ImplementJobs();
                        gt = 1;
                        return gt;
                    }
                    else{
                        *found=0;
                        return 1;
                    }
                }
                else
                {
                    ImplementJobsForR();
                    gt = 1;
                    return gt;
                }
            }
            else if (strcmp(ar[1], "-s") == 0)
            {
                if (c1 > 2)
                {
                    if (strcmp(ar[2], "-r") == 0)
                    {
                        ImplementJobs();
                        gt = 1;
                        return gt;
                    }
                    else
                    {
                        *found=0;
                        return 1;
                    }
                }
                else
                {
                    ImplementJobsForS();
                    gt = 1;
                    return gt;
                }
            }
            else if(strcmp(ar[1],"-rs")==0)
            {
                *found=1;
                gt=1;
                ImplementJobs();
            }
            else
            {
                *found = 0;
                return 1;
            }
        }
        else{
            *found=1;
            gt=1;
            ImplementJobs();
        }
    }
    return gt;
}

int cmpfunc(const void*a,const void* b)
{
    struct background_process* ia=(struct background_process*)a;
    struct background_process* ib=(struct background_process*)b;

    return(strcmp((*ia).name,(*ib).name));
}

void ImplementJobs()
{
    struct background_process tmp[100];
    int c=0;
    for(int i=0;i<bg_count;i++)
    {
        char status[100];
        char buf[250];
        sprintf(buf, "/proc/%d", bg_process[i].pid);
        FILE *fp = fopen(buf, "r");
        if (fp != NULL)
        {   
            char* temp=getProcessStat(bg_process[i].pid);
            if(strcmp(temp,"R")==0 || strcmp(temp,"S")==0)
            {
                strcpy(bg_process[i].status,"Running");
            }
            if(strcmp(temp,"T")==0)
            {
                strcpy(bg_process[i].status,"Stopped");
            }
            tmp[c++]=bg_process[i];
        }
    }
    qsort(tmp,c,sizeof(struct background_process),cmpfunc);
    for(int i=0;i<c;i++)
    {
        printf("[%d] %s %s [%d]\n",tmp[i].job_count,tmp[i].status,tmp[i].name,tmp[i].pid);
    }
}

void ImplementJobsForR()
{
    struct background_process tmp[100];
    int c=0;
    for(int i=0;i<bg_count;i++)
    {
        char status[100];
        char buf[250];
        sprintf(buf, "/proc/%d", bg_process[i].pid);
        FILE *fp = fopen(buf, "r");
        if (fp != NULL)
        {   
            char* temp=getProcessStat(bg_process[i].pid);
            if(strcmp(temp,"R")==0 || strcmp(temp,"S")==0)
            {
                strcpy(bg_process[i].status,"Running");
                tmp[c++]=bg_process[i];
            }
            if(strcmp(temp,"T")==0)
            {
                strcpy(bg_process[i].status,"Stopped");
            }
        }
    }
    qsort(tmp,c,sizeof(struct background_process),cmpfunc);
    for(int i=0;i<c;i++)
    {
        printf("[%d] %s %s [%d]\n",tmp[i].job_count,tmp[i].status,tmp[i].name,tmp[i].pid);
    }

}

void ImplementJobsForS()
{
    struct background_process tmp[100];
    int c=0;
    for(int i=0;i<bg_count;i++)
    {
        char status[100];
        char buf[250];
        sprintf(buf, "/proc/%d", bg_process[i].pid);
        FILE *fp = fopen(buf, "r");
        if (fp != NULL)
        {   
            char* temp=getProcessStat(bg_process[i].pid);
            if(strcmp(temp,"R")==0 || strcmp(temp,"S")==0)
            {
                strcpy(bg_process[i].status,"Running");
            }
            if(strcmp(temp,"T")==0)
            {
                strcpy(bg_process[i].status,"Stopped");
                tmp[c++]=bg_process[i];
            }
        }
    }
    qsort(tmp,c,sizeof(struct background_process),cmpfunc);
    for(int i=0;i<c;i++)
    {
        printf("[%d] %s %s [%d]\n",tmp[i].job_count,tmp[i].status,tmp[i].name,tmp[i].pid);
    }
    
}
