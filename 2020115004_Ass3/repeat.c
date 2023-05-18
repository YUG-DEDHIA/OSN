#include "headers.h"
int repeat(char *token, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h,int* prev_stdin_fd,int* prev_stdout_fd)
{
    int cd_ = 0;
    int s = 0; //0 for failing in cd implementation 1 for passing
    char temp[250];
    int bg = 0;
    int found = 0;
    int found1 = 0;
    int status = ManageInput(token); //-1 for empty input

    if (status != -1)
    {
        
        char args[100][100];
        int c1 = TokeizeWithSpace(token, args, &bg);
        char **ar;  //final array after input redirection
        char **arg; // array before input redirection

        if (strcmp(args[0], "exit") == 0)
            return 2;

        arg = (char **)malloc(sizeof(char *) * c1);
        for (int i = 0; i < c1; i++)
        {
            arg[i] = (char *)malloc(sizeof(char) * strlen(args[i]));
            strcpy(arg[i], args[i]);
        }

        //pipes inmplementation
        int gt_pipe = pipe_check(arg,c1,&found,bg,curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h,prev_stdin_fd, prev_stdout_fd);
        if (gt_pipe == 1)
            goto end;

        // I/O redirection
        // gt_io=2 if change in stdin has occured and gt_io=3 if change has occured in stdout and gt_io==5 if both change has occured.
        //gt_io=-1 for wrong file path
        int gt_io = IOchangeCheck(arg, &c1, token);
        if (gt_io == 1)
            goto end;
        if (gt_io == -1)
        {
            found = 1;
            goto end;
        }

        ar = (char **)malloc(sizeof(char *) * c1);
        for (int i = 0; i < c1; i++)
        {
            ar[i] = (char *)malloc(sizeof(char) * strlen(args[i]));
            strcpy(ar[i], args[i]);
        }

        // cd implementation
        int gt = check_cd(token, curr_dir, start_dir, prev_old_pwd, old_pwd, &s, &found, &cd_);
        if (gt == 1)
        {
            if (s == 1 && cd_ == 0)
            {
                strcpy(prev_old_pwd, old_pwd);
                strcpy(old_pwd, getcwd(temp, sizeof(temp)));
            }
            goto end;
        }
        //echo implementation
        int gt_echo = echo_check(token, &found);
        if (gt_echo == 1)
            goto end;
        //pwd implementation
        if (token[0] == 'p' && token[1] == 'w' && token[2] == 'd')
        {
            if (token[3] == '\0')
            {
                found = 1;
                printf("%s\n", getcwd(temp, sizeof(temp)));
                goto end;
            }
            if (token[3] == ' ' && token[4] == '\0')
            {
                found = 1;
                printf("%s\n", getcwd(temp, sizeof(temp)));
                goto end;
            }
        }

        //ls implementation
        int gt_ls = ls_check(token, start_dir, &found);
        if (gt_ls == 1)
            goto end;

        //pinfo implementation
        int gt_pinfo = pinfo_check(token, &found, start_dir);
        if (gt_pinfo == 1)
            goto end;

        //history implementation
        int gt_his = check_history(ar, c1, &found, history, c_h);
        if (gt_his == 1)
            goto end;

        // foreground and background implementation
        int chk = CheckSystemProcess(ar, &found1, bg);
        if (chk == 0)
        {
            printf("%s: command not found\n", token);
            return -1;
        }

    end:

        cd_ = 0;
        s = 0;
        if (found == 0 && found1 == 0)
        {
            printf("%s: command not found\n", token);
        }
        if (gt_io == 2 || gt_io == 5)
        {
            fflush(stdin); // VERY IMPORTANT!!!!
            dup2(*prev_stdin_fd, fileno(stdin));
        }
        if (gt_io == 3 || gt_io == 5)
        {
            //===================== Asigning the prev_stdout to stdout====================
            fclose(stdout);
            stdout = fdopen(*prev_stdout_fd, "w"); // fdopen just opens the descriptor without flushing
            *prev_stdout_fd = dup(fileno(stdout));
        }
        gt_io = 0;
    }
    return 1;
}

int check_repeat(char *token, char *curr_dir, char *start_dir, char prev_old_pwd[], char old_pwd[], char history[][10000], int c_h, int *found,int* prev_stdin_fd,int* prev_stdout_fd)
{
    int gt = 0;
    if (token[0] == 'r' && token[1] == 'e' && token[2] == 'p' && token[3] == 'e' && token[4] == 'a' && token[5] == 't')
    {
        if (token[6] == '\0')
        {
            gt = 1;
            return gt;
        }
        if (token[6] == ' ')
        {
            if (token[7] == '\0')
            {
                gt = 1;
                return gt;
            }
            if (token[7] != '\0')
            {
                int i = 7;
                char ch1[100];
                int c = 0;
                while (token[i] != ' ' && token[i] != '\0')
                {
                    ch1[c++] = token[i++];
                }
                ch1[c] = '\0';
                int x = ChangStrToInt(ch1);
                if (token[i] == '\0')
                {
                    gt = 1;
                    return gt;
                }
                if (token[i] = ' ')
                {
                    char ch[1000];
                    int c = 0;
                    *found = 1;
                    for (int j = i + 1; j < strlen(token); j++)
                    {
                        ch[c++] = token[j];
                    }
                    ch[c] = '\0';
                    for (int j = 0; j < x; j++)
                    {
                        int child_check = repeat(ch, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h,prev_stdin_fd,prev_stdout_fd);
                        if (child_check == -1)
                            return -1;
                    }
                    gt = 1;
                    return gt;
                }
                else
                {
                    gt = 1;
                    return gt;
                }
            }
            else
            {
                gt = 1;
                return gt;
            }
        }
    }
    return gt;
}