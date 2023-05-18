#ifndef __SYSTEM_H__
#define __SYSTEM_H__

struct background_process{
    pid_t pid;
    char name[250];
};

int TokeizeWithSpace(char *token, char args[][100],int *bg);
int CheckSystemProcess(char** args,int* found,int bg);

#endif