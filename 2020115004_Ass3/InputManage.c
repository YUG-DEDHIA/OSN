#include "headers.h"

int ManageInput(char* ch)
{
    int n =strlen(ch);
    char *temp =(char*)malloc(sizeof(char)*n+1);

    int c=0;
    int i=0;
    while(ch[i]==' ')
    {
        i++;
        if(i == n)
        {
            return -1;
        }
    }
    for(;i<n;i++)
    {  
        temp[c++]=ch[i];     
        if(ch[i]==' ')
        {
            while(ch[i]==' ')
            {
                i++;
                if(i==n)
                {
                    break;
                }
            }
            i--;
        }        
    }
    temp[c]='\0';
    strcpy(ch,temp);
}

int TokeizeWithSpace(char *token, char args[][100], int *bg)
{
    char temp4[200];
    strcpy(temp4, token);
    int n = strlen(temp4);

    int c1 = 0, c2 = 0;
    if (temp4[n - 1] == ' ')
    {
        temp4[n - 1] = '\0';
        n--;
    }
    for (int i = 0; i <= n; i++)
    {
        if (temp4[i] == '&')
        {
            *bg = 1;
            i++;
        }
        if (temp4[i] == ' ' || temp4[i] == '\0')
        {
            args[c1++][c2] = '\0';
            c2 = 0;
        }
        else
            args[c1][c2++] = temp4[i];
        
    }
    strcpy(args[c1], "\0");
    if(strcmp(args[c1-1],"\0")==0)
        c1--;
    return c1;
}

int ChangStrToInt(char *ch)
{
    int n = strlen(ch);
    int x = 0;
    int c = 1;
    while (n--)
    {
        x += c * (ch[n] - 48);
        c *= 10;
    }
    return x;
}

char* CreateToken(char** arg,int n)
{
    char *ch=malloc(sizeof(char)*10000);
    strcpy(ch,"");
    for(int i=0;i<n;i++)
    {
        int c=strlen(ch);
        if(c!=0)
        {
            ch[c]=' ';
            ch[c+1]='\0';
        }
        strcat(ch,arg[i]);
    }
    return ch;
}