#ifndef __PINFO_H__
#define __PINFO_H__

int pinfo_check(char* token,int* found,char * start_dir);
void ImplementPinfo(char *start_dir);
void ImplementPinfoWithpid(int pid,char *star_dir);
int ChangStrToInt(char*ch);

char *getProcessStat(int pid);
char* getVirtualMemory(int pid);
char *getPath(int pid);
#endif