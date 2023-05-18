#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h> ////This one is used for -ls command in ls
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

#include "prompt.h"
#include "InputManage.h"
#include "InBuilt.h"
#include "LS.h"
#include "display.h"
#include "system.h"
#include "pinfo.h"
#include "history.h"
struct background_process bg_process[10000];
int bg_count;

void handler(int sig)
{
    int status;
    int i = 0;
    while (i < bg_count)
    {
        if (waitpid(bg_process[i].pid, &status, WNOHANG) != 0 && bg_process[i].pid != 0)
        {
            if (WIFEXITED(status))
                printf("\n%s with pid= %d has been exited normally\n", bg_process[i].name, bg_process[i].pid);
            else
                printf("\n%s with pid= %d has been exited abnormally\n", bg_process[i].name, bg_process[i].pid);
            bg_process[i].pid = 0;
        }
        i++;
    }
    return;
}

int main()
{
    bg_count = 0;

    FILE *fp = fopen("history.txt", "a+");
    char history[20][10000];
    int c_h = 0;
    fclose(fp);
    int num_of_lines = get_num_of_lines("history.txt");
    c_h = num_of_lines;
    read_history_from_file("history.txt", history, num_of_lines);

    char temp[250], buf[250];
    char curr_dir[100] = "~";
    char *start_dir = getcwd(buf, sizeof(buf));
    int u10 = 0;
    char prev_old_pwd[100];
    char old_pwd[100];
    int cd_ = 0;
    int s = 0; // 0 for failing in cd implementation 1 for passing
    strcpy(old_pwd, start_dir);
    strcpy(prev_old_pwd, "OLDPWD not set");

    signal(SIGCHLD, handler);
    while (1)
    {
        char input[10000];
        int u30=0;
        displayprompt(curr_dir);
        gets(input);
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

                char args[100][100];
                int u31=0;
                int c1 = TokeizeWithSpace(token, args, &bg);
                char **ar;
                ar = (char **)calloc(c1, sizeof(char *));
                int i = 0;
                while (i < c1)
                {
                    ar[i] = (char *)calloc(strlen(args[i]), sizeof(char));
                    strcpy(ar[i], args[i]);
                    i++;
                }
                strcpy(history[c_h % 20], token);
                c_h++;

                if (strcmp(ar[0], "exit") == 0)
                    goto finish;

                // cd implementation
                int gt = check_cd(token, curr_dir, start_dir, prev_old_pwd, old_pwd, &s, &found, &cd_);
                if (gt == 1)
                {
                    if (s == 1)
                    {
                        if (cd_ == 0)
                        {
                            strcpy(prev_old_pwd, old_pwd);
                            strcpy(old_pwd, getcwd(temp, sizeof(temp)));
                        }
                    }
                    goto end;
                }

                // echo implementation
                int gt_echo = echo_check(token, &found);
                if (gt_echo == 1)
                    goto end;

                // pwd implementation
                if (token[0] == 'p' && token[1] == 'w' && token[2] == 'd')
                {
                    if (token[3] == '\0')
                    {
                        found = 1;
                        int nf = 0;
                        printf("%s\n", getcwd(temp, sizeof(temp)));
                        goto end;
                    }
                    if (token[3] == ' ')
                    {
                        if (token[4] == '\0')
                        {
                            found = 1;
                            printf("%s\n", getcwd(temp, sizeof(temp)));
                            goto end;
                        }
                    }
                }

                // ls implementation
                int gt_ls = ls_check(token, start_dir, &found);
                if (gt_ls == 1)
                    goto end;

                // pinfo implementation
                int gt_pinfo = pinfo_check(token, &found, start_dir);
                if (gt_pinfo == 1)
                    goto end;

                // dispaly implementation
                int gt_display = display_check(token, &found, start_dir);
                if (gt_display == 1)
                    goto end;

                // history implementation
                int gt_his = check_history(ar, c1, &found, history, c_h);
                if (gt_his == 1)
                    goto end;

                // foreground and background implementation
                int chk = CheckSystemProcess(ar, &found1, bg);
                if (chk == 0)
                {
                    printf("%s: Error\n", token);
                    return 0;
                }

            end:

                cd_ = 0;
                s = 0;
                if (found == 0 && found1 == 0)
                {
                    printf("%s: Error\n", token);
                }
            }
            token = strtok(NULL, ";");
        }
    }
finish:
    write_history_into_file("history.txt", history, c_h);
}
