#include "headers.h"

int pinfo_check(char *token, int *found, char *start_dir)
{
    int gt = 0;
    if (token[0] == 'p' && token[1] == 'i' && token[2] == 'n' && token[3] == 'f' && token[4] == 'o')
    {
        if (token[5] == '\0')
        {
            *found = 1;
            ImplementPinfo(start_dir);
            gt = 1;
            return gt;
        }
        if (token[5] == ' ')
        {
            *found = 1;
            if (token[6] == '\0')
            {
                ImplementPinfo(start_dir);
                gt = 1;
                return gt;
            }
            else
            {
                int i = 6;
                char ch[100];
                int c = 0;
                while (token[i] != ' ' && token[i] != '\0')
                {
                    ch[c++] = token[i++];
                }
                ch[c] = '\0';
                if (token[i] == ' ' && token[i + 1] != '\0')
                {
                    *found = 0;
                    return 1;
                }
                ImplementPinfoWithpid(ChangStrToInt(ch), start_dir);

                gt = 1;
            }
        }
    }
    return gt;
}

void ImplementPinfo(char *start_dir)
{
    char buf[250];
    int pid = getpid();
    sprintf(buf, "/proc/%d", pid);
    FILE *fp = fopen(buf, "r");
    if (fp == NULL)
    {
        printf("No such process exists\n");
        return;
    }
    else
    {
        printf("pid -- %d\n", pid);
        printf("Process Status -- R+\n");
        printf("memory -- %s{Virtual Memory}\n", getVirtualMemory(pid));
        char *ch = getPath(pid);
        if (ch != NULL)
        {
            check_syntax(ch, start_dir);
            printf("Executable Path -- %s\n", ch);
        }
    }
    fclose(fp);
}

char *getProcessStat(int pid)
{
    char ch[1000];
    char *res = (char *)malloc(sizeof(char) *2);
    sprintf(ch, "/proc/%d/status", pid);
    FILE *fp = fopen(ch, "r");
    char ch1 = fgetc(fp);
    char temp[1000];
    int c = 0;
    while (ch1 != EOF)
    {
        if (ch1 == '\t' || ch1 == '\n' || ch1==' ')
        {
            temp[c] = '\0';
            if (strcmp("State:", temp) == 0)
            {
                res[0] = fgetc(fp);
            }
            c = 0;
            temp[c] = '\0';
        }
        else
            temp[c++] = ch1;
        ch1 = fgetc(fp);
    }
    fclose(fp);
    res[1] = '\0';
    return res;
}

char *getVirtualMemory(int pid)
{
    char ch[1000];
    char *res = (char *)malloc(sizeof(char) * 100);
    sprintf(ch, "/proc/%d/status", pid);
    FILE *fp = fopen(ch, "r");
    char ch1 = fgetc(fp);
    char temp[100];
    int c = 0;
    int count = 0;
    while (ch1 != EOF)
    {
        if (ch1 == '\t' || ch1 == '\n' )
        {
            count--;
            temp[c] = '\0';
            if (count == 0)
            {
                strcpy(res, temp);
                break;
            }
            if (strcmp("VmSize:", temp) == 0)
            {
                count = 1;
            }
            c = 0;
            temp[c] = '\0';

        }
        else 
            temp[c++] = ch1;
        ch1 = fgetc(fp);
    }
    fclose(fp);
    return res;
}

char *getPath(int pid)
{
    char filename[250];
    sprintf(filename, "/proc/%d/exe", pid);
    int size = 100;
    char *buffer = NULL;

    while (1)
    {
        buffer = (char *)realloc(buffer, size);
        int nchars = readlink(filename, buffer, size);
        if (nchars < 0)
        {
            free(buffer);
            printf("Unable to use readlink\n");
            return NULL;
        }
        if (nchars < size)
            return buffer;
        size *= 2;
    }
}

void ImplementPinfoWithpid(int pid, char *star_dir)
{
    char buf[250];
    sprintf(buf, "/proc/%d", pid);
    FILE *fp = fopen(buf, "r");
    if (fp == NULL)
    {
        printf("No such process exists\n");
        return;
    }
    else
    {
        printf("pid -- %d\n", pid);
        if(pid==getpid())
            printf("Process Status -- R+\n");
        else   
            printf("Process Status -- %s\n", getProcessStat(pid));
        printf("memory -- %s{Virtual Memory}\n", getVirtualMemory(pid));
        char *ch = getPath(pid);
        if (ch != NULL)
        {
            check_syntax(ch, star_dir);
            printf("Executable Path -- %s\n", ch);
        }
    }
    fclose(fp);
}
