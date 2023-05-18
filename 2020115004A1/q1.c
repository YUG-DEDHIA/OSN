#include<stdio.h> 
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h> 
#include<string.h>
#define BUFF_SIZE 1000

void swap(char *a, char *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
    char prog1[100];

    // error handling for numbner of arguments
    if (argc < 2) {
        sprintf(prog1,"Enter the file name!\n");
        write(1,prog1,strlen(prog1));
        return -1; // exit with error if there is no txt file given
    }
    if (argc > 2) {
        sprintf(prog1,"Too many arguments!\n");
        write(1,prog1,strlen(prog1));
        return -1; // exit with error if there is too many arguments
    }

    // open the input file
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror("Error in opening input file");
        return -1;
    }

    //making the directory
    int dir = mkdir("Assignment", 0777);

    // create and open the output file
    int fd2 = open("Assignment/1_A.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd2 < 0) {
        perror("Error in opening output file");
        return -1;
    }

    // reverse the contents of the input 
    // file and write to the output file
    char prog[100];
    char *buffer = (char *)malloc(BUFF_SIZE*sizeof(char));    
    int N = lseek(fd1, 0, SEEK_END);
    int temp = N;
    while (temp > BUFF_SIZE) {
        sprintf(prog, "\rPercentage Complete: %d", ((N-temp)*100)/N);
        write(1, prog, strlen(prog));
        // printf("\r");
        // double exp = ((N-temp)/(double)(N))*100;
        // printf("Percentage Complete: %0.2f", exp);
        int buff_size = BUFF_SIZE;
        lseek(fd1, -buff_size, SEEK_CUR);
        read(fd1, buffer, buff_size);
        lseek(fd1, -buff_size, SEEK_CUR);
        int s = 0, e = buff_size-1;
        while (s < e) {
            swap(&buffer[s], &buffer[e]);
            s++; e--;
        }
        write(fd2, buffer, buff_size);
        temp = temp - buff_size;
    }
    int buff_size = temp;
    lseek(fd1, -buff_size, SEEK_CUR);
    read(fd1, buffer, buff_size);
    lseek(fd1, -buff_size, SEEK_CUR);
    int s = 0, e = buff_size-1;
    while (s < e) {
        swap(&buffer[s], &buffer[e]);
        s++; e--;
    }

    write(fd2, buffer, buff_size);
    temp = temp-buff_size;

    

    sprintf(prog, "\rPercentage Complete: %d", ((N-temp)*100)/N);
    write(1, prog, strlen(prog));

    write(1, "\n", 1);

    // printf("\r");
    // double exp = ((N-temp)/(double)(N))*100;
    // printf("Percentage Complete: %0.2f", exp);
    // printf("\n");
    
    return 0;
}
