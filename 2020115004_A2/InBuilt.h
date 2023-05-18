#ifndef __INBUILT_H__
#define __INBUILT_H__

int check_cd(char *token,char* curr_dir, char* start_dir,char* prev_old_pwd,char* old_pwd,int *s,int *found,int* cd_);
int ImplementCd(char* dest,char* curr_loc,char* abs_start_loc);
void check_syntax(char *ch,char * ch2);

int echo_check(char *token,int* found);
void ImplementEcho(char* ch);

#endif