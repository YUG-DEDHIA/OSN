#include "headers.h"

int main()
{
    //Managing global variables
    bg_count = 0;
    job_count = 1;
    active_process = 0;
    fg_count=0;

    //Managing History
    FILE *fp = fopen("history.txt", "a+");
    char history[20][10000];
    int c_h = 0;
    fclose(fp);
    int num_of_lines = get_num_of_lines("history.txt");
    c_h = num_of_lines;
    read_history_from_file("history.txt", history, num_of_lines);
    //=================================================================

    //cd requirements
    char temp[250], buf[250];
    char curr_dir[100] = "~";
    char *start_dir = getcwd(buf, sizeof(buf));
    char prev_old_pwd[100];
    char old_pwd[100];
    int cd_ = 0;
    int s = 0; //0 for failing in cd implementation 1 for passing
    strcpy(old_pwd, start_dir);
    strcpy(prev_old_pwd, "OLDPWD not set");
    //=================================================================

    signal(SIGCHLD, child_handler);
    signal(SIGINT, CtrlC_handler);
    signal(SIGTSTP, CtrlZ_handler);

    // Converting FILE* to fd then storing it's duplicate
    int prev_stdin_fd = dup(fileno(stdin));
    int prev_stdout_fd = dup(fileno(stdout));

    while (1)
    {

        char input[10000];
        strcpy(input,"\0");
        //Ctrl-D implementation
        if (feof(stdin) == 1)
        {
            int c = getc(stdin);
            if (c == EOF)
            {
                printf("exit\n");
                goto finish;
            }
            ungetc(c, stdin);
        }
        x=0;
        displayprompt(curr_dir);
        fgets(input, 10000, stdin);
        int n = strlen(input);
        if (input[n - 1] == '\n')
            input[n - 1] = '\0';
        char *token;
        token = strtok(input, ";");
        while (token != NULL)
        {
            int bg = 0;

            int found = 0;
            int found1 = 0;

            int status = ManageInput(token); //-1 for empty input
            if (status != -1)
            {
                int gt_io = 0;
                strcpy(history[c_h % 20], token);
                c_h++;

                char args[100][100];
                int c1 = TokeizeWithSpace(token, args, &bg);
                char **ar;  //final array after input redirection
                char **arg; // array before input redirection

                arg = (char **)malloc(sizeof(char *) * c1);
                for (int i = 0; i < c1; i++)
                {
                    arg[i] = (char *)malloc(sizeof(char) * strlen(args[i]));
                    strcpy(arg[i], args[i]);
                }

                if (strcmp(arg[0], "exit") == 0)
                    goto finish;

                //pipes inmplementation
                int gt_pipe = pipe_check(arg, c1, &found, bg, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &prev_stdin_fd, &prev_stdout_fd);
                if (gt_pipe == 1)
                    goto end;

                // I/O redirection
                // gt_io=2 if change in stdin has occured and gt_io=3 if change has occured in stdout and gt_io==5 if both change has occured.
                //gt_io=-1 for wrong file path
                gt_io = IOchangeCheck(arg, &c1, token);
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
                    ar[i] = (char *)malloc(sizeof(char) * strlen(args[i]) + 1);
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

                // repeat implementation
                int gt_rep = check_repeat(token, curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &found, &prev_stdin_fd, &prev_stdout_fd);
                if (gt_rep == 1)
                    goto end;
                if (gt_rep == -1)
                    return 0;
                if (gt_rep == 2)
                    goto finish;

                //history implementation
                int gt_his = check_history(ar, c1, &found, history, c_h);
                if (gt_his == 1)
                    goto end;

                //jobs implementation
                int gt_jobs = check_jobs(ar, c1, &found);
                if (gt_jobs == 1)
                    goto end;

                //sig implementation
                int gt_sig = check_sig(ar, c1, &found);
                if (gt_sig == 1)
                    goto end;

                // bg and fg implementation
                int gt_bg_fg = check_bg_fg(ar, c1, &found);
                if (gt_bg_fg == 1)
                    goto end;
                
                //replay implementation
                int gt_replay = check_replay(ar,c1,&found,curr_dir, start_dir, prev_old_pwd, old_pwd, history, c_h, &prev_stdin_fd, &prev_stdout_fd);
                if(gt_replay==1)
                    goto end;

                // foreground and background implementation
                int chk = CheckSystemProcess(ar, &found1, bg);
                if (chk == 0)
                {
                    printf("%s: command not found\n", token);
                    return 0;
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
                    dup2(prev_stdin_fd, fileno(stdin));
                }
                if (gt_io == 3 || gt_io == 5)
                {

                    //===================== Asigning the prev_stdout to stdout====================
                    fclose(stdout);
                    stdout = fdopen(prev_stdout_fd, "w"); // fdopen just opens the descriptor without flushing
                    prev_stdout_fd = dup(fileno(stdout));
                }
            }
            token = strtok(NULL, ";");
        }
    }
finish:
    write_history_into_file("history.txt", history, c_h);
}
