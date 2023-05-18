#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define LEN 500000
int GetFileDetails(struct stat stats, char *name, int ret)
{
    char ch[100] = {0};
    sprintf(ch, "%s is created: ", name);
    write(1, ch, 100);
    if (strcmp("Directory", name) == 0)
    {
        if (stats.st_mode && S_ISDIR(stats.st_mode))
        {
            write(1, "YES\n", 4);
        }
        else
        {
            write(1, "NO\n", 3);
            return -1;
        }
    }
    else
    {
        if (!(ret))
        {
            write(1, "YES\n", 4);
        }
        else
        {
            write(1, "NO\n", 3);
            return -1;
        }
    }
    sprintf(ch, "User has read permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IRUSR)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "User has write permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IWUSR)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "User has execute permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IXUSR)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);

    sprintf(ch, "Group has read permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IRGRP)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "Group has write permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IWGRP)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "Group has execute permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IXGRP)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);

    sprintf(ch, "Others has read permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IROTH)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "Others has write permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IWOTH)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
    sprintf(ch, "Others has execute permission on %s: ", name);
    write(1, ch, 100);
    if (stats.st_mode & S_IXOTH)
        write(1, "YES\n", 4);
    else
        write(1, "NO\n", 3);
}
void revstr(char *ch, int l)
{
    char ch1;
    for (int i = 0; i < (l / 2); i++)
    {
        ch1 = ch[i];
        ch[i] = ch[l - 1 - i];
        ch[l - 1 - i] = ch1;
    }
}
int CheckRev(int fd1, int fd2, long long int fs)
{

    long long int FileSize = fs;

    char c[LEN + 1];
    char c2[LEN + 1];
    int ret = 1;
    while (fs > 0)
    {
        long long int fac = fs / LEN;

        if (fs % LEN == 0)
        {
            lseek(fd1, ((fac - 1) * LEN), SEEK_SET);
            read(fd1, c, LEN);
            read(fd2, c2, LEN);
            revstr(c, LEN);
            if(strcmp(c, c2) != 0)
            {
                ret=0;
                return 0;
            }
            fs -= LEN;
        }
        else
        {
            lseek(fd1, fac * LEN, SEEK_SET);
            read(fd1, c, fs % LEN);
            read(fd2, c2, fs % LEN);
            revstr(c, fs % LEN);
            if(strcmp(c, c2) != 0)
            {
                ret=0;
                return 0;
            }
            fs -= (fs % LEN);
        }
    }
    return ret;
}

int main(int argc, char *argv[])
{
    // Generating output file name and output file path
    int check, i = 0;
    struct stat stats;
    if (argc != 4)
    {
        perror("Incorrect no. of arguments");
        return 0;
    }
    stat(argv[3], &stats);
    GetFileDetails(stats, "Directory", 0);
    int re1 = stat(argv[1], &stats);
    GetFileDetails(stats, "newfile", re1);
    int re2 = stat(argv[2], &stats);
    GetFileDetails(stats, "oldfile", re2);

    if ((re1) == 0 && (re2) == 0)
    {
        char ch[100] = "Whether file contents are reversed in newfile: ";
        write(1, ch, strlen(ch));
        int fd1 = open(argv[1], O_RDONLY);
        if (fd1 < 0)
        {
            perror("Error in opening new file");
            return -1;
        }
        int fd2 = open(argv[2], O_RDONLY);
        if (fd2 < 0)
        {
            perror("Error in opening old file");
            return -1;
        }
        stat(argv[1], &stats);
        long long int fs = stats.st_size;
        stat(argv[2], &stats);
        if (fs != stats.st_size)
        {
            write(1, "NO\n", 3);
            return -1;
        }
        else
        {
            if (CheckRev(fd1, fd2, fs) == 1)
            {
                write(1, "YES\n", 4);
            }
            else
            {
                write(1, "NO\n", 3);
            }
        }
    }
    else
    {
        write(1, "Either of the files are missing\n", 32);
        return -1;
    }
}