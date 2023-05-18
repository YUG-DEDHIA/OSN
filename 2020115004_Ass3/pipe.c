#include "headers.h"

int pipe_check(char **arg, int n, int *found, int bg, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd)
{
    int num_of_pipes = 0, gt = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(arg[i], "|") == 0)
        {
            if (arg[i + 1] != NULL)
                num_of_pipes++;
        }
    }
    if (num_of_pipes == 0)
        return gt;
    if (num_of_pipes > 0)
    {
        int pipes_index[num_of_pipes];
        int c = 0;
        for (int i = 0; i < n; i++)
        {
            if (strcmp(arg[i], "|") == 0)
            {
                pipes_index[c++] = i;
            }
        }
        *found = 1;
        struct operands *op = (struct operands *)malloc(sizeof(struct operands) * num_of_pipes);
        for (int i = 0; i < num_of_pipes; i++)
        {
            if (i == 0)
            {
                op[i].first_part = (char **)malloc(sizeof(char *) * pipes_index[i]);
                for (int j = 0; j < (pipes_index[i]); j++)
                {
                    op[i].first_part[j] = (char *)malloc(sizeof(char) * strlen(arg[j]));
                    strcpy(op[i].first_part[j], arg[j]);
                }
            }
            if (i > 0)
            {
                op[i].first_part = (char **)malloc(sizeof(char *) * (pipes_index[i] - pipes_index[i - 1] - 1));
                for (int j = pipes_index[i - 1] + 1; j < (pipes_index[i]); j++)
                {
                    op[i].first_part[j - pipes_index[i - 1] - 1] = (char *)malloc(sizeof(char) * strlen(arg[j]));
                    strcpy(op[i].first_part[j - pipes_index[i - 1] - 1], arg[j]);
                }
            }
            if (i + 1 == num_of_pipes)
            {
                op[i].second_part = (char **)malloc(sizeof(char *) * n - 1 - pipes_index[i]);
                for (int j = pipes_index[i] + 1; j < n; j++)
                {
                    op[i].second_part[j - pipes_index[i] - 1] = (char *)malloc(sizeof(char) * strlen(arg[j]));
                    strcpy(op[i].second_part[j - pipes_index[i] - 1], arg[j]);
                }
            }
            else
            {
                op[i].second_part = (char **)malloc(sizeof(char *) * pipes_index[i + 1] - pipes_index[i] - 1);
                for (int j = pipes_index[i] + 1; j < pipes_index[i + 1]; j++)
                {
                    op[i].second_part[j - pipes_index[i] - 1] = (char *)malloc(sizeof(char) * strlen(arg[j]));
                    strcpy(op[i].second_part[j - pipes_index[i] - 1], arg[j]);
                }
            }
        }
        ImplementPipes(op, pipes_index, bg, num_of_pipes, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, prev_stdin_fd, prev_stdout_fd);
        gt = 1;
    }
    return gt;
}

void ImplementPipes(struct operands *op, int* pipes_index, int bg, int num_of_pipes, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd)
{
    int fd[num_of_pipes][2];
    int status, found = 0;

    int c1 = 0, f2 = 0, f1 = 0;
    char **first_part;
    char **last_part;
    while (op[0].first_part[c1] != NULL)
    {
        if (strcmp(op[0].first_part[c1], "<") == 0)
        {
            f1 = 1;
            break;
        }
        c1++;
    }
    if (f1 == 1)
    {
        first_part = (char **)malloc(sizeof(char *) * c1);
        for (int i = 0; i < c1; i++)
        {
            first_part[i] = (char *)malloc(sizeof(char) * strlen(op[0].first_part[i]));
            strcpy(first_part[i], op[0].first_part[i]);
        }
    }
    else
        first_part = op[0].first_part;
    int c2 = 0;
    while (op[num_of_pipes - 1].second_part[c2] != NULL)
    {
        if (strcmp(op[num_of_pipes - 1].second_part[c2], ">") == 0)
        {
            f2 = 1;
            break;
        }
        if (strcmp(op[num_of_pipes - 1].second_part[c2], ">>") == 0)
        {
            f2 = 2;
            break;
        }
        c2++;
    }
    if (f2 == 1 || f2 == 2)
    {
        last_part = (char **)malloc(sizeof(char *) * c2);
        for (int i = 0; i < c2; i++)
        {
            last_part[i] = (char *)malloc(sizeof(char) * strlen(op[num_of_pipes - 1].second_part[i]));
            strcpy(last_part[i], op[num_of_pipes - 1].second_part[i]);
        }
    }
    else
        last_part = op[num_of_pipes - 1].second_part;

    int forkRet = fork();
    if (forkRet == 0)
    {
        int f[num_of_pipes + 1];
        for (int i = 0; i < num_of_pipes; i++)
        {
            if (pipe(fd[i]) == -1)
            {
                printf("Unable to create pipe\n");
                return;
            }
        }
        for (int i = 0; i <= num_of_pipes; i++)
        {
            f[i] = fork();
            if (i == 0)
            {
                if (f[i] == 0)
                {

                    if (f1 == 1 && op[0].first_part[c1 + 1] != NULL)
                    {
                        FILE *temp;
                        temp = fopen(op[0].first_part[c1 + 1], "r");
                        if (temp == NULL)
                        {
                            printf("%s:No such file or directory\n", op[0].first_part[c1 + 1]);
                            return;
                        }
                        fclose(temp);
                        temp = freopen(op[0].first_part[c1 + 1], "r", stdin);
                    }

                    dup2(fd[i][1], STDOUT_FILENO);
                    for (int j = 0; j < num_of_pipes; j++)
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                    char *temp=CreateToken(first_part,c1);
                    int gt_jobs = check_jobs(first_part, c1, &found);
                    if (gt_jobs == 1)
                        kill(getpid(),SIGKILL);
                    int gt_his = check_history(first_part, c1, &found, history, c_h);
                    if (gt_his == 1)
                        kill(getpid(),SIGKILL);
                    int gt_rep = check_repeat(temp, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &found, prev_stdin_fd, prev_stdout_fd);
                    if (gt_rep == 1 || gt_rep == -1 || gt_rep == 2)
                        kill(getpid(),SIGKILL);
                    int gt_pinfo = pinfo_check(temp, &found, start_dir);
                    if (gt_pinfo == 1)
                        kill(getpid(),SIGKILL);
                    execvp(first_part[0], first_part);
                    //func(op[0].first_part,first, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, prev_stdin_fd, prev_stdout_fd);
                }
            }
            else if (i == num_of_pipes)
            {
                if (f[i] == 0)
                {

                    if ((f2 == 1 || f2 == 2) && op[i - 1].second_part[c2 + 1] != NULL)
                    {
                        int fd = open(op[i - 1].second_part[c2 + 1], O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                        close(fd);
                        if (f2 == 1)
                        {
                            FILE *temp;
                            temp = fopen(op[i - 1].second_part[c2 + 1], "w");
                            if (temp == NULL)
                            {
                                printf("%s:No such file or directory\n", op[i - 1].second_part[c2 + 1]);
                                return;
                            }
                            fclose(temp);
                            temp = freopen(op[i - 1].second_part[c2 + 1], "w", stdout);
                        }
                        else
                        {
                            FILE *temp;
                            temp = fopen(op[i - 1].second_part[c2 + 1], "a");
                            if (temp == NULL)
                            {
                                printf("%s:No such file or directory\n", op[i - 1].second_part[c2 + 1]);
                                return;
                            }
                            fclose(temp);
                            temp = freopen(op[i - 1].second_part[c2 + 1], "a", stdout);
                        }
                    }

                    dup2(fd[i - 1][0], STDIN_FILENO);
                    for (int j = 0; j < num_of_pipes; j++)
                    {
                        close(fd[j][0]);
                        close(fd[j][1]);
                    }
                    char* temp=CreateToken(last_part,c2);
                    int gt_jobs = check_jobs(last_part, c2, &found);
                    if (gt_jobs == 1)
                        kill(getpid(),SIGKILL);
                    int gt_his = check_history(last_part, c2, &found, history, c_h);
                    if (gt_his == 1)
                        kill(getpid(),SIGKILL);
                    int gt_rep = check_repeat(temp, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &found, prev_stdin_fd, prev_stdout_fd);
                    if (gt_rep == 1 || gt_rep == -1 || gt_rep == 2)
                        kill(getpid(),SIGKILL);
                    int gt_pinfo = pinfo_check(temp, &found, start_dir);
                    if (gt_pinfo == 1)
                        kill(getpid(),SIGKILL);
                    execvp(last_part[0], last_part);
                    // func(op[i - 1].second_part,l, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, prev_stdin_fd, prev_stdout_fd);
                }
                break;
            }
            else
            {

                if (f[i] == 0)
                {
                    dup2(fd[i - 1][0], STDIN_FILENO);
                    dup2(fd[i][1], STDOUT_FILENO);
                    for (int j = 0; j < num_of_pipes; j++)
                    {
                        close(fd[j][0]);
                        close(fd[j][0]);
                    }
                    char *temp=CreateToken(op[i].first_part,pipes_index[i] - pipes_index[i - 1] - 1);
                    int gt_jobs = check_jobs(op[i].first_part, pipes_index[i] - pipes_index[i - 1] - 1, &found);
                    if (gt_jobs == 1)
                        kill(getpid(),SIGKILL);
                    int gt_his = check_history(op[i].first_part, pipes_index[i] - pipes_index[i - 1] - 1, &found, history, c_h);
                    if (gt_his == 1)
                        kill(getpid(),SIGKILL);
                    int gt_rep = check_repeat(temp, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &found, prev_stdin_fd, prev_stdout_fd);
                    if (gt_rep == 1 || gt_rep == -1 || gt_rep == 2)
                        kill(getpid(),SIGKILL);
                    int gt_pinfo = pinfo_check(temp, &found, start_dir);
                    if (gt_pinfo == 1)
                        kill(getpid(),SIGKILL);
                    execvp(op[i].first_part[0], op[i].first_part);
                }
            }
            waitpid(f[i], NULL, 0);
        }
        for (int i = 0; i < num_of_pipes; i++)
        {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        waitpid(f[num_of_pipes], NULL, 0);
        kill(getpid(), SIGKILL);
    }
    if (bg == 0)
    {
        strcpy(fg_process[fg_count].name, op[0].first_part[0]);
        fg_process[fg_count++].pid = forkRet;
        while (waitpid(forkRet, &status, WNOHANG) == 0 && x == 0)
        {
        }
    }
    if (bg == 1)
    {
        strcpy(bg_process[bg_count].name, op[0].first_part[0]);
        bg_process[bg_count].pid = forkRet;
        if (active_process == 0)
        {
            job_count = 1;
        }
        bg_process[bg_count].job_count = job_count;
        active_process++;
        bg_count++;
        job_count++;
        printf("%d\n", forkRet);
    }
}

// void func(char **arg, int c1, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *prev_stdin_fd, int *prev_stdout_fd)
// {
//     //Creating token
//     char *token = (char *)malloc(sizeof(char) * 10000);
//     strcpy(token, "");
//     for (int i = 0; i < c1; i++)
//     {
//         int n = strlen(token);
//         if (n != 0)
//         {
//             token[n] = ' ';
//             token[n + 1] = '\0';
//         }
//         strcat(token, arg[c1]);
//         printf("%s\n", token);
//     }

//     int cd_ = 0;
//     int s = 0; //0 for failing in cd implementation 1 for passing
//     char temp[250];
//     int found = 0;
//     int found1 = 0;
//     int status = ManageInput(token); //-1 for empty input

//     if (status != -1)
//     {

//         char **ar; //final array after input redirection

//         if (strcmp(arg[0], "exit") == 0)
//             goto finish;

//         // I/O redirection
//         // gt_io=2 if change in stdin has occured and gt_io=3 if change has occured in stdout and gt_io==5 if both change has occured.
//         //gt_io=-1 for wrong file path
//         int gt_io = IOchangeCheck(arg, &c1, token);
//         if (gt_io == 1)
//             goto end;
//         if (gt_io == -1)
//         {
//             found = 1;
//             goto end;
//         }

//         ar = (char **)malloc(sizeof(char *) * c1);
//         for (int i = 0; i < c1; i++)
//         {
//             ar[i] = (char *)malloc(sizeof(char) * strlen(arg[i]));
//             strcpy(ar[i], arg[i]);
//         }

//         // cd implementation
//         int gt = check_cd(token, curr_dir, start_dir, prev_old_pwd, old_pwd, &s, &found, &cd_);
//         if (gt == 1)
//         {
//             if (s == 1 && cd_ == 0)
//             {
//                 strcpy(prev_old_pwd, old_pwd);
//                 strcpy(old_pwd, getcwd(temp, sizeof(temp)));
//             }
//             goto end;
//         }
//         //echo implementation
//         int gt_echo = echo_check(token, &found);
//         if (gt_echo == 1)
//             goto end;
//         //pwd implementation
//         if (token[0] == 'p' && token[1] == 'w' && token[2] == 'd')
//         {
//             if (token[3] == '\0')
//             {
//                 found = 1;
//                 printf("%s\n", getcwd(temp, sizeof(temp)));
//                 goto end;
//             }
//             if (token[3] == ' ' && token[4] == '\0')
//             {
//                 found = 1;
//                 printf("%s\n", getcwd(temp, sizeof(temp)));
//                 goto end;
//             }
//         }

//         //ls implementation
//         int gt_ls = ls_check(token, start_dir, &found);
//         if (gt_ls == 1)
//             goto end;

//         //pinfo implementation
//         int gt_pinfo = pinfo_check(token, &found, start_dir);
//         if (gt_pinfo == 1)
//             goto end;

//         //history implementation
//         int gt_his = check_history(ar, c1, &found, history, c_h);
//         if (gt_his == 1)
//             goto end;

//         // foreground and background implementation
//         found1 = 1;
//         int stat = execvp(ar[0], ar);
//         if (stat == -1)
//         {
//             printf("%s: command not found\n", token);
//             goto finish;
//         }

//     end:

//         cd_ = 0;
//         s = 0;
//         if (found == 0 && found1 == 0)
//         {
//             printf("%s: command not found\n", token);
//         }
//         if (gt_io == 2 || gt_io == 5)
//         {
//             fflush(stdin); // VERY IMPORTANT!!!!
//             dup2(*prev_stdin_fd, fileno(stdin));
//         }
//         if (gt_io == 3 || gt_io == 5)
//         {
//             //===================== Asigning the prev_stdout to stdout====================
//             fclose(stdout);
//             stdout = fdopen(*prev_stdout_fd, "w"); // fdopen just opens the descriptor without flushing
//             *prev_stdout_fd = dup(fileno(stdout));
//         }
//         gt_io = 0;
//     }
// finish:
//     kill(getpid(), SIGKILL);
// }
