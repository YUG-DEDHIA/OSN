#ifndef __REPEAT_H__
#define __REPEAT_H__

int check_repeat(char *token, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h,int *found,int* prev_stdin_fd,int* prev_stdout_fd);
int repeat(char *token,char* ch,char* start_dir,char prev_old_pwd[],char old_pwd[],char history[][10000],int c_h,int* prev_stdin_fd,int* prev_stdout_fd);

#endif