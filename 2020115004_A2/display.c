#include "headers.h"

int display_check(char *token, int *found, char *start_dir)
{
    int gt = 0;
    if (token[0] == 'd' && token[1] == 'i' && token[2] == 's' && token[3] == 'c' && token[4] == 'o' && token[5] == 'v' && token[6] == 'e' && token[7] == 'r')
    {

        if (token[8] == '\0')
        {
            *found = 1;

            ImplementDisplay(".", 1, 1, start_dir);
            gt = 1;
            return gt;
        }
        if (token[8] == ' ')
        {
            *found = 1;
            if (token[9] == '\0')
            {

                ImplementDisplay(".", 1, 1, start_dir);
                gt = 1;
                return gt;
            }
            else
            {
                int i = 9;
                char ch1[100];
                int c1 = 0;
                char ch2[100];
                int c2 = 0;
                int st = 0;
                int flagd = 0, flagf = 0;
                while (token[i] != '\0')
                {
                    if (token[i] == ' ')
                    {
                        st = 1;
                        i++;
                        continue;
                    }
                    else if (token[i] == '-' && token[i + 1] != ' ' && token[i + 1] != '\0')
                    {
                        if (token[i + 1] == 'f' && (token[i + 2] == '\0'))
                        {
                            flagf = 1;
                            i++;
                        }
                        if (token[i + 1] == 'd' && (token[i + 2] == '\0'))
                        {
                            flagd = 1;
                            i++;
                        }
                        if (token[i + 2] != '\0')
                        {
                            if (token[i + 1] == 'f' && (token[i + 2] == ' '))
                            {
                                flagf = 1;
                                i++;
                            }
                            if (token[i + 1] == 'd' && (token[i + 2] == ' '))
                            {
                                flagd = 1;
                                i++;
                            }
                            if ((token[i + 1] == 'f' && token[i + 2] == 'd') || (token[i + 2] == 'f' && token[i + 1] == 'd'))
                            {
                                flagf = 1;
                                flagd = 1;
                                i += 2;
                            }
                        }
                        st = 1;
                    }
                    else if (token[i] == '\"')
                    {
                        st = 1;
                        while (token[i] != '\0')
                        {
                            if (token[i] == '\"' || token[i] == ' ')
                            {
                                i++;
                                continue;
                            }
                            ch2[c2++] = token[i++];
                        }
                        ch2[c2] = '\0';
                        break;
                    }
                    else if (st == 0 && token[i] != '\0')
                    {
                        ch1[c1++] = token[i];
                    }
                    i++;
                }
                ch1[c1] == '\0';
                if (flagf == 0 && flagd == 0)
                {
                    flagf = 1;
                    flagd = 1;
                }
                char *dest;
                if (c1 == 0)
                {

                    dest = ".";
                }
                else
                {
                    dest = malloc(sizeof(char) * c1);
                    for (int i = 0; i < c1; i++)
                    {
                        dest[i] = ch1[i];
                    }
                    dest[c1] = '\0';
                }

                if (c2 == 0)
                {
                    ImplementDisplay(dest, flagd, flagf, start_dir);
                }
                else
                {
                    char *fil = malloc(sizeof(char) * c2);
                    for (int i = 0; i < c2; i++)
                        fil[i] = ch2[i];
                    fil[c2]='\0';

                    ImplementDisplayWithFile(dest, flagd, flagf, fil, start_dir);
                }

                gt = 1;
            }
        }
    }
    return gt;
}


void ImplementDisplay(char *dir, int f1, int f2, char *s)
{
    DIR *d;
    struct dirent *myfile;
    struct stat mystat;
    char buf[512], temp[512];
    char *b;
    if (dir[0] == '~')
    {

        int n = (strlen(s) + (strlen(dir) - 1));
        b = malloc(sizeof(char) * n);
        for (int i = 0; i < n; i++)
        {
            if (i < strlen(s))
            {
                b[i] = s[i];
            }
            else
            {
                b[i] = dir[i - strlen(s) + 1];
            }
        }
        b[n] = '\0';
        d = opendir(b);
    }
    else
    {
        b = dir;
        d = opendir(b);
    }

    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open %s\n", dir);
        return;
    }
    if (f1 == 1)
    {
        printf("%s\n",dir);
    }

    while ((myfile = readdir(d)) != NULL)
    {
        if (myfile->d_name[0] != '.')
        {
            sprintf(buf,"%s/%s",b,myfile->d_name);
            stat(buf, &mystat);
            if (mystat.st_mode & S_ISDIR(mystat.st_mode))
            {

                sprintf(temp, "%s/%s", dir, myfile->d_name);

                ImplementDisplay(temp, f1, f2, s);
            }
            else
            {
                if (f2 == 1)
                {
                    printf("%s/%s\n",dir, myfile->d_name);
                }
            }
        }
    }
    closedir(d);
}

void ImplementDisplayWithFile(char *dir, int f1, int f2, char *fil, char *s)
{
    DIR *d;
    struct dirent *myfile;
    struct stat mystat;
    char buf[512], temp[512];
    char *b;
    if (dir[0] == '~')
    {

        int n = (strlen(s) + (strlen(dir) - 1));
        b = malloc(sizeof(char) * n);
        for (int i = 0; i < n; i++)
        {
            if (i < strlen(s))
            {
                b[i] = s[i];
            }
            else
            {
                b[i] = dir[i - strlen(s) + 1];
            }
        }
        b[n] = '\0';
        d = opendir(b);
    }
    else
    {
        b = dir;
        d = opendir(b);
    }

    if (d == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open %s\n", dir);
        return;
    }
    if (f1 == 1)
    {
        int slash_index=-1;

        for(int i=0;i<strlen(dir);i++)
        {
            if(dir[i]=='/'){
                slash_index=i;
            }
        }
        if(slash_index==-1)
        {
            if(strcmp(dir,fil)==0)
                printf("%s\n",dir);
        }
        else
        {
            char* lat=malloc(sizeof(char)*(strlen(dir)-slash_index-1));

            for(int i=slash_index+1;i<strlen(dir);i++)
            {
                lat[i-slash_index-1]=dir[i];
            }
            lat[strlen(dir)-slash_index-1]='\0';
            // printf("%s\n",lat);
            if(strcmp(lat,fil)==0)
                printf("%s\n", dir);
        }
        
    }

    while ((myfile = readdir(d)) != NULL)
    {
        if (myfile->d_name[0] != '.')
        {
            sprintf(buf,"%s/%s",b,myfile->d_name);
            stat(buf, &mystat);
            if (mystat.st_mode & S_ISDIR(mystat.st_mode))
            {

                sprintf(temp,"%s/%s",dir, myfile->d_name);

                ImplementDisplayWithFile(temp, f1, f2, fil, s);
            }
            else
            {
                if (f2 == 1 && strcmp(myfile->d_name, fil) == 0)
                {
                    printf("%s/%s\n",dir, myfile->d_name);
                }
            }
        }
    }
    closedir(d);
}

// int ChangStrToInt(char *ch)
// {
//     int n = strlen(ch);
//     int x = 0;
//     int c = 1;
//     while (n--)
//     {
//         x += c * (ch[n] - 48);
//         c *= 10;
//     }
//     return x;
// }
