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

    while(i<n)
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
        i++;   
    }
    temp[c]='\0';
    strcpy(ch,temp);
}