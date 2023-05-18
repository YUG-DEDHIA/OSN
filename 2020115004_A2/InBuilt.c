#include "headers.h"

void check_syntax(char *ch, char *ch1)
{
    char temp[100];
    temp[0] = '~';
    int n = strlen(ch);
    int c = 0;
    int c1 = 1;
    int n1 = strlen(ch1);
    int u9 = 0;
    int u10 = 0;
    if (n < n1)
    {
        return;
    }
    if (n > n1)
    {
        int i = 0;
        while (i < n1)
        {
            if (ch[i] == ch1[i])
                c++;
            i++;
        }
        if (c == n1)
        {
            int j = n1;
            while (j < n)
            {
                if (j == n1 && ch[j] == '/' && ch[j + 1] == '\0')
                {
                    j++;
                    break;
                }
                temp[c1++] = ch[j];
                j++;
            }
            temp[c1] = '\0';
            strcpy(ch, temp);
        }
        else
            return;
    }
}

int check_cd(char *token, char *curr_dir, char *start_dir, char *prev_old_pwd, char *old_pwd, int *s, int *found, int *cd_)
{
    int gt = 0;
    if (token[0] == 'c' && token[1] == 'd')
    {
        // handling flags
        if (token[2] == '\0')
        {
            *found = 1;
            *s = 1;
            int chek1 = chdir(start_dir);
            if (chek1 != -1)
            {
                strcpy(curr_dir, "~");
                gt = 1;
                return gt;
            }
            if (chek1 = -1)
            {
                printf("Cannot change the directory\n");
                gt = 1;
                return gt;
            }
        }
        if (token[2] == ' ')
        {
            *s = 1;
            *found = 1;
            if (token[3] == '.')
            {
                if (token[4] == ' ' && token[5] == '\0')
                {
                    gt = 1;
                    return gt;
                }

                else if (token[4] == '\0')
                {
                    gt = 1;
                    return gt;
                }
            }
            if (token[3] == '~' || token[3] == '\0')
            {
                if (token[3] == '~' && token[4] == ' ' && token[5] == '\0')
                {
                    int chek2 = chdir(start_dir);
                    if (chek2 != -1)
                    {
                        strcpy(curr_dir, "~");
                        gt = 1;
                        return gt;
                    }
                    if (chek2 = -1)
                    {
                        gt = 1;
                        return gt;
                    }
                }
                else if (token[3] == '~' && token[4] == '\0')
                {
                    int chek2 = chdir(start_dir);
                    if (chek2 != -1)
                    {
                        strcpy(curr_dir, "~");
                        gt = 1;
                        return gt;
                    }
                    if (chek2 = -1)
                    {
                        printf("Cannot change the directory\n");
                        gt = 1;
                        return gt;
                    }
                }
            }

            if (token[3] == '-')
            {
                if (token[4] == ' ' && token[5] == '\0')
                {
                    if (strcmp(prev_old_pwd, "OLDPWD not set") != 0)
                    {
                        *cd_ = 1;
                        int chek2 = chdir(prev_old_pwd);
                        if (chek2 != -1)
                        {
                            strcpy(curr_dir, prev_old_pwd);
                            printf("%s\n", prev_old_pwd);
                            strcpy(prev_old_pwd, old_pwd);
                            strcpy(old_pwd, curr_dir);
                            check_syntax(curr_dir, start_dir);
                            gt = 1;
                            return gt;
                        }
                        if (chek2 = -1)
                        {
                            printf("Unable to change the directory\n");
                            gt = 1;
                            return gt;
                        }
                    }
                    else
                    {
                        printf("%s\n", prev_old_pwd);
                        gt = 1;
                        return gt;
                    }
                }
                else if (token[4] == '\0')
                {
                    if (strcmp(prev_old_pwd, "OLDPWD not set") != 0)
                    {
                        *cd_ = 1;
                        int chek2 = chdir(prev_old_pwd);
                        if (chek2 != -1)
                        {
                            strcpy(curr_dir, prev_old_pwd);
                            printf("%s\n", prev_old_pwd);
                            strcpy(prev_old_pwd, old_pwd);
                            strcpy(old_pwd, curr_dir);
                            check_syntax(curr_dir, start_dir);
                            gt = 1;
                            return gt;
                        }
                        if (chek2 = -1)
                        {
                            printf("Unable to change the directory\n");
                            gt = 1;
                            return gt;
                        }
                    }
                    else
                    {
                        printf("%s\n", prev_old_pwd);
                        gt = 1;
                        return gt;
                    }
                }
            }

            char arg[100];
            int c = 0;
            while (token[c + 3] != ' ' && token[c + 3] != '\0')
            {
                arg[c] = token[c + 3];
                c++;
            }
            arg[c] = '\0';
            if (token[c + 3] == ' ' && token[c + 4] != '\0')
            {
                *s = 0;
                printf("cd: It has too many arguments\n");
                gt = 1;
                return gt;
            }
            int stat = -1;
            stat = ImplementCd(arg, curr_dir, start_dir); //-1 for not a directory
            check_syntax(curr_dir, start_dir);
            if (stat == -1)
            {
                *s = 0;
                printf("cd: %s: is not a directory\n", arg);
            }
            gt = 1;
            return gt;
        }
    }
    return gt;
}

int ImplementCd(char *dest, char *curr_loc, char *abs_start_loc)
{
    int n = strlen(dest);
    if (dest[0] == '"' && dest[n - 1] == '"')
    {
        dest[n - 1] = '\0';
        dest += 1;
    }
    n = strlen(dest);
    if (dest[n - 1] == '/')
        dest[n - 1] = '\0';
    char buf[250];
    char *abs_init_loc = getcwd(buf, sizeof(buf));
    if (strcmp(dest, "..") == 0)
    {
        char *p = strrchr(abs_init_loc, '/');
        p[0] = '\0';
        int check = chdir(abs_init_loc);
        if (check == -1)
        {
            return -1;
        }
        if (check != -1)
        {
            strcpy(curr_loc, abs_init_loc);
            return 0;
        }
    }
    int check = chdir(dest);
    if (check == -1)
    {
        return -1;
    }
    if (check != -1)
    {
        char *abs_curr_loc = getcwd(buf, sizeof(buf));
        strcpy(curr_loc, abs_curr_loc);
    }
}


int echo_check(char *token, int *found)
{
    int gt = 0;
    if (token[0] == 'e' && token[1] == 'c' && token[2] == 'h' && token[3] == 'o')
    {
        int gt = 0;
        char arg[1000];
        int i = 5, c = 0;
        if (token[4] == '\0')
        {
            *found = 1;
            arg[0] = ' ';
            arg[1] = '\0';
            ImplementEcho(arg);
            gt = 1;
            return gt;
        }
        if (token[4] == ' ')
        {
            *found = 1;
            if (token[5] == '\0')
            {
                arg[0] = ' ';
                arg[1] = '\0';
                ImplementEcho(arg);
                gt = 1;
                return gt;
            }

            while (token[i] != '\0')
            {
                arg[c++] = token[i++];
            }
            arg[c] = '\0';
            ImplementEcho(arg);
            gt = 1;
            return gt;
        }
    }
    return gt;
}
void ImplementEcho(char *ch)
{
    printf("%s\n", ch);
}