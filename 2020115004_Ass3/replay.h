#ifndef __REPLAY_H__
#define __REPLAY_H__

int check_replay(char **ar, int n, int *found, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h,int* prev_stdin_fd,int* prev_stdout_fd);

#endif