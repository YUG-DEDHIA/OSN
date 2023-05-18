#include "headers.h"

void displayprompt(char ch[])
{
    char *username = getenv("USER");
    char sysname[1000]; 
    int check=gethostname(sysname,1000);
    if(check==-1)
        perror("hostname");
    printf("%s@%s:%s> ",username,sysname,ch);

}