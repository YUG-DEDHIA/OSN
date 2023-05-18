#include "headers.h"

int IOchangeCheck(char** a,int *c,char * token)
{
    int inp_change=0; 
    int out_change=0; 
    int gt=0; // < == 2 ; > == 3 ; >> == 3 ; < and > == 5
    int found=0;
    int c1=0;
    char * new_token=(char*)malloc(sizeof(char)*10000);
    strcpy(new_token,"");
    for(int i=0;i<*c;i++)
    {
        
        if(strcmp(a[i],"<")==0)
        {
            if(a[i+1]!=NULL)
            {
                found=1;
                if(ChangeInputStream(a[i+1])==-1)
                {
                    return -1;
                }
                inp_change=2;
            }
            else
            {
                gt=1;
                return gt;
            }
        }
        else if(strcmp(a[i],">")==0)
        {
            if(a[i+1]!=NULL)
            {
                found=1;
                ChangeOutputStream(a[i+1],1);
                out_change= 3;
            }
            else
            {
                gt=1;
                return gt;
            }
        }
        else if(strcmp(a[i],">>")==0)
        {
            if(a[i+1]!=NULL)
            {
                found=1;
                ChangeOutputStream(a[i+1],2);
                out_change= 3;
            }
            else
            {
                gt=1;
                return gt;
            }
        }
        if(found==0)
        {
            int n= strlen(new_token);
            if(n!=0)
            {
                new_token[n]=' ';
                new_token[n+1]='\0';
            }
            strcat(new_token,a[i]);
            c1=i+1;
        }
    }
    strcpy(token,new_token);
    *c=c1;
    gt=inp_change+out_change;
    return gt;
}

int ChangeInputStream(char *pathname)
{
    FILE *temp;
    temp=fopen(pathname,"r");
    if(temp == NULL)
    {
        printf("%s:No such file or directory\n",pathname);
        return -1;
    }
    fclose(temp);
    temp =freopen(pathname,"r",stdin);
    
    
}

void ChangeOutputStream(char *pathname,int type)
{
    if(type==1)
    {
        int fd=open(pathname,O_CREAT,S_IRUSR | S_IWUSR |S_IRGRP | S_IROTH);
        close(fd);
        FILE *temp=freopen(pathname,"w",stdout);
        if(temp == NULL)
        {
            printf("%s:No such file or directory\n",pathname);
            return;
        }
    }
    else if(type == 2)
    {
        int fd=open(pathname,O_CREAT,S_IRUSR | S_IWUSR |S_IRGRP | S_IROTH);
        close(fd);
        FILE *temp=freopen(pathname,"a",stdout);
        if(temp == NULL)
        {
            printf("%s:No such file or directory\n",pathname);
            return;
        }
    }
}