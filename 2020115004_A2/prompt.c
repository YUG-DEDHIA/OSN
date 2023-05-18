#include "headers.h"
#define SIZE 500

void displayprompt(char ch[]){
    char user[SIZE], host[SIZE], cwd[SIZE], home[SIZE], quit[SIZE], laterPart[SIZE], previous[SIZE], fg_processName[SIZE];
    getlogin_r(user,sizeof(user));
    gethostname(host,sizeof(host));
    getcwd(cwd,sizeof(cwd));
    int l = strlen(home);
    char* check = strstr(cwd,home);
    if(check != NULL){
        printf("<%s@%s:~%s>",user,host,ch);
    }
    if(check==NULL){
        printf("<%s@%s:%s>",user,host,ch);
    }
}
