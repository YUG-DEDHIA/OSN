#include "headers.h"

int ls_check(char *token,char* start_dir, int *found)
{
    char temp[250];
    int gt=0;
    if (token[0] == 'l' && token[1] == 's')
    {
        if (token[2] == '\0' || token[2] == ' ')
        {
            char flags[50];
            *found = 1;
            int a = 0;
            int l = 0;
            int c = 0;
            char directories[20][100];
            strcpy(directories[0], "No External directories");
            char *cwd = getcwd(temp, sizeof(temp));
            if (token[2] == '\0')
            {
                ImplementLS(cwd);
                gt=1;return gt;
            }
            int n = strlen(token);
            if (token[3] == '\0')
            {
                ImplementLS(cwd);
                gt=1;return gt;
            }
            for (int i = 3; i < n; i++)
            {

                if (token[i] == '-')
                {
                    while (token[i] != ' ' && token[i] != '\0')
                    {
                        if (token[i] == 'a')
                            a = 1;
                        else if (token[i] == 'l')
                            l = 1;
                        if (token[i] != 'a' && token[i] != 'l' && token[i] != '-')
                        {
                            *found = 0;
                            gt=1;return gt;
                        }
                        i++;
                    }
                }
                if (token[i] != ' ' && token[i] != '\0' && token[i] != '-')
                {
                    int count = 0;
                    while (token[i] != ' ' && token[i] != '\0')
                    {

                        directories[c][count++] = token[i];
                        i++;
                    }
                    directories[c++][count] = '\0';
                }
            }
            if (c == 0)
            {
                Implementls(a, l, cwd);
                gt=1;return gt;
            }
            else if (c > 0)
            {
                if (c == 1)
                {
                    if (strcmp(directories[0], ".") == 0)
                    {
                        Implementls(a, l, cwd);
                        gt=1;return gt;
                    }
                    if (strcmp(directories[0], "..") == 0)
                    {
                        char temp3[100];
                        strcpy(temp3, cwd);
                        char *p = strrchr(temp3, '/');
                        p[0] = '\0';
                        Implementls(a, l, temp3);
                        gt=1;return gt;
                    }
                    if (strcmp(directories[0], "~") == 0)
                    {
                        Implementls(a, l, start_dir);
                        gt=1;return gt;
                    }
                }
                for (int i = 0; i < c; i++)
                {
                    printf("%s:\n", directories[i]);
                    Implementls(a, l, directories[i]);
                    gt=1;
                }
            }
        }
    }
    return gt;
}


void Implementls(int a, int l, char *ch)
{
    if (a == 1 && l == 1)
    {
        ImplementLSWithAL(ch);
    }
    if (a == 1 && l != 1)
    {
        ImplementLSWithA(ch);
    }
    if (a != 1 && l == 1)
    {
        ImplementLSWithL(ch);
    }
    if (a != 1 && l != 1)
    {
        ImplementLS(ch);
    }
}

void ImplementLS(char *ch)
{
    struct dirent *dir;
    int n;
    DIR *d = opendir(ch);
    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n");
        return;
    }
    while ((dir = readdir(d)) != NULL)
    {
        char *name = dir->d_name;
        if (name[0] != '.')
            printf("%s\n", name);
    }

    closedir(d);
}

void ImplementLSWithA(char *ch)
{
    struct dirent *dir;
    int n;
    DIR *d = opendir(ch);
    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n");
        return;
    }
    while ((dir = readdir(d)) != NULL)
        printf("%s\n", dir->d_name);

    closedir(d);
}

void ImplementLSWithAL(char *ch)
{
    DIR *d;
    struct dirent *myfile;
    struct stat mystat;

    char buf[512];
    d = opendir(ch);
    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n");
        return;
    }
    while ((myfile = readdir(d)) != NULL)
    {
        sprintf(buf, "%s/%s", ch, myfile->d_name);
        stat(buf, &mystat);
        if (mystat.st_mode & S_ISDIR(mystat.st_mode))
            printf("d");
        else
            printf("-");

        printf((mystat.st_mode & S_IRUSR) ? "r" : "-");
        printf((mystat.st_mode & S_IWUSR) ? "w" : "-");
        printf((mystat.st_mode & S_IXUSR) ? "x" : "-");
        printf((mystat.st_mode & S_IRGRP) ? "r" : "-");
        printf((mystat.st_mode & S_IWGRP) ? "w" : "-");
        printf((mystat.st_mode & S_IXGRP) ? "x" : "-");
        printf((mystat.st_mode & S_IROTH) ? "r" : "-");
        printf((mystat.st_mode & S_IWOTH) ? "w" : "-");
        printf((mystat.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %ld", mystat.st_nlink); // get number of hard links

        struct passwd *tf = getpwuid(mystat.st_uid);
        printf(" %s", tf->pw_name);

        struct group *gf = getgrgid(mystat.st_gid);
        printf(" %s", gf->gr_name);
        printf(" %ld", mystat.st_size);

        char fileTime[100];
        struct tm *time = localtime(&(mystat.st_mtime));
        strftime(fileTime, 99, "%b %d %H:%M", time);
        printf("\t%s\t", fileTime);

        printf(" %s\n", myfile->d_name);
    }
    closedir(d);
}

void ImplementLSWithL(char *ch)
{
    DIR *d;
    struct dirent *myfile;
    struct stat mystat;

    char buf[512];
    d = opendir(ch);
    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory\n");
        return;
    }
    while ((myfile = readdir(d)) != NULL)
    {
        if (myfile->d_name[0] != '.')
        {
            sprintf(buf, "%s/%s", ch, myfile->d_name);
            stat(buf, &mystat);
            if (mystat.st_mode & S_ISDIR(mystat.st_mode))
                printf("d");
            else
                printf("-");

            printf((mystat.st_mode & S_IRUSR) ? "r" : "-");
            printf((mystat.st_mode & S_IWUSR) ? "w" : "-");
            printf((mystat.st_mode & S_IXUSR) ? "x" : "-");
            printf((mystat.st_mode & S_IRGRP) ? "r" : "-");
            printf((mystat.st_mode & S_IWGRP) ? "w" : "-");
            printf((mystat.st_mode & S_IXGRP) ? "x" : "-");
            printf((mystat.st_mode & S_IROTH) ? "r" : "-");
            printf((mystat.st_mode & S_IWOTH) ? "w" : "-");
            printf((mystat.st_mode & S_IXOTH) ? "x" : "-");

            printf(" %ld", mystat.st_nlink); // get number of hard links

            struct passwd *tf = getpwuid(mystat.st_uid);
            printf(" %s", tf->pw_name);

            struct group *gf = getgrgid(mystat.st_gid);
            printf(" %s", gf->gr_name);
            printf(" %ld", mystat.st_size);

            char fileTime[100];
            struct tm *time = localtime(&(mystat.st_mtime));
            strftime(fileTime, 99, "%b %d %H:%M", time);
            printf("\t%s\t", fileTime);

            printf(" %s\n", myfile->d_name);
        }
    }
    closedir(d);
}
