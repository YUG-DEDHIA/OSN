#include "headers.h"

int get_num_of_lines(char* filename)
{
    FILE* fp=fopen(filename,"r");
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
    int c=0;
    char ch=fgetc(fp); 
    while(ch!=EOF)
    {
        if(ch=='\n')
            c++;
        ch=fgetc(fp);
    }
    fclose(fp);
    return c;
}

void read_history_from_file(char* filename,char history[][10000],int num)
{
     FILE* fp=fopen(filename,"r");
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return;
    }
    if(num==0)
        return;
    int c=0,c1=0;
    char temp[10000];
    char ch=fgetc(fp);
    while(ch!=EOF)
    {
        if(ch=='\n')
        {
            temp[c1]='\0';
            strcpy(history[c++],temp);
            strcpy(temp,"\0");
            c1=0;
        }
        else
            temp[c1++]=ch;
        ch=fgetc(fp);

    }

    fclose(fp);
}

void write_history_into_file(char* filename,char history[][10000], int num)
{
     FILE* fp=fopen(filename,"w+");
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return;
    }
    int x=(num<20) ? num : 20;
    for(int i=0;i<x;i++)
    {
        fprintf(fp,"%s\n",history[i]);
    }

    fclose(fp);
}

int check_history(char ** argv, int argc, int * found, char history[][10000], int c_h)
{
    int gt=0;
    if(strcmp(argv[0],"history")==0)
    {
        if( argc > 2)
        {
            gt =1;
            return gt;
        }
        else{
            *found=1;
            if(argc==1)
            {
                int x=((c_h<=10) ? c_h : 10);
                calc_history(history,c_h,x);
                gt=1;
                return gt;
            }
            else if(argc==2)
            {
                int x=ChangStrToInt(argv[1]);
                if(x>c_h)
                {
                    x=c_h;
                }
                calc_history(history,c_h,x);
                gt=1;
                return gt;
            }
        }
    }
    return gt;
}

void calc_history(char history[][10000], int c_h, int x)
{
    int temp=c_h;
    for(int i=c_h-x;i<c_h;i++)
    {
        printf("%s\n",history[i%20]);
    }
}