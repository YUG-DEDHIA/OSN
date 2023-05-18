#ifndef __PIPES_H__
#define __PIPES_H__

struct operands
{
    char **first_part;
    char **second_part;
};

int pipe_check(char **arg,int n,int* found,int bg,char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd);
void ImplementPipes(struct operands *arg,int* pipes_index, int bg, int num_of_pipes,char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd);
//void func(char **arg,int c1, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd);

#endif