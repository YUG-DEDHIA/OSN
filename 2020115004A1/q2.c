#include<stdio.h> 
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h> 
#define BUFF_SIZE 2

void swap(char *a, char *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
    char prog[100];
    // error handling for numbner of arguments
    if (argc < 4) {
        char prog1[100];
        sprintf(prog1,"Enter the file name!\n");
        write(1,prog1,strlen(prog1));
        return -1; // exit with error if there is no txt file given
    }
    if (argc > 4) {
        char prog1[100];
        printf("Too many arguments!\n");
        write(1,prog1,strlen(prog1));
        return -1; // exit with error if there is too many arguments
    }
    
    // error handling for number of characters
    int var1 = atoi(argv[2]);
    int var2 = atoi(argv[3]);
    if (var1 > var2) {
        char prog2[100];
        sprintf(prog2,"Invalid input!\n");
        write(1,prog2,strlen(prog2));
        return -1;
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

    // get the number of characters in the input file
    int total_char = lseek(fd1, 0, SEEK_END);
    lseek(fd1, 0, SEEK_SET);

    // reverse the contents of the first part of the 
    // input file and write to the output file
    char *buffer = (char *)malloc(BUFF_SIZE*sizeof(char));    
    int N = lseek(fd1, var1, SEEK_CUR);
    int temp = N;
    while (temp > BUFF_SIZE) {
        // printf("\r");
        double exp = ((total_char-temp)/(double)(total_char))*100;
        sprintf(prog, "\rPercentage Complete: %d", ((N-temp)*100)/N);
        write(1, prog, strlen(prog));
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
        temp = temp - BUFF_SIZE;
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
    // printf("\r");
    double exp = ((total_char-temp)/(double)(total_char))*100;
    sprintf(prog, "\rPercentage Complete: %d", ((total_char-temp)*100)/total_char);
        write(1, prog, strlen(prog));
    // printf("Percentage Complete: %0.2f", exp);

    // write the contents of the second part of the input
    lseek(fd1, var1, SEEK_SET);
    temp = var2 - var1 + 1;
    while (temp > BUFF_SIZE) {
        int buff_size = BUFF_SIZE;
        read(fd1, buffer, buff_size);
        write(fd2, buffer, buff_size);
        temp = temp - BUFF_SIZE;
        // printf("\r");
        double exp = ((var2-temp)/(double)(total_char))*100;
        sprintf(prog, "\rPercentage Complete: %d", ((var2-temp)*100)/total_char);
        write(1, prog, strlen(prog));
        // printf("Percentage Complete: %0.2f", exp);
    }
    
    buff_size = temp;
    read(fd1, buffer, buff_size);
    write(fd2, buffer, buff_size);
    temp = temp-buff_size;
    
    // printf("\r");
    exp = ((var2-temp)/(double)(total_char))*100;
    sprintf(prog, "\rPercentage Complete: %d", ((var2-temp)*100)/total_char);
        write(1, prog, strlen(prog));
    // printf("Percentage Complete: %0.2f", exp);

    // reverse the contents of the third part of the input
    lseek(fd1, 0, SEEK_END);
    temp = total_char - var2-1;
    while (temp > BUFF_SIZE) {
        // printf("\r");
        double exp = ((total_char-temp)/(double)(total_char))*100;
        sprintf(prog, "\rPercentage Complete: %d", ((total_char-temp)*100)/total_char);
        write(1, prog, strlen(prog));
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
        temp = temp - BUFF_SIZE;
    }
    
    buff_size = temp;
    lseek(fd1, -buff_size, SEEK_CUR);
    read(fd1, buffer, buff_size);
    lseek(fd1, -buff_size, SEEK_CUR);
    s = 0; 
    e = buff_size-1;
    while (s < e) {
        swap(&buffer[s], &buffer[e]);
        s++; e--;
    }
    write(fd2, buffer, buff_size);
    temp = temp-buff_size;
    
    // printf("\r");
    exp = ((total_char-temp)/(double)(total_char))*100;
    sprintf(prog, "\rPercentage Complete: %d", ((total_char-temp)*100)/total_char);
        write(1, prog, strlen(prog));
    // printf("Percentage Complete: %0.2f", exp);

    printf("\n");

    // close the input and output files
    if (close(fd1) < 0) {
        perror("Error in closing input file");
    }
    if (close(fd2) < 0) {
        perror("Error in closing output file");
    }
    close(fd1);
    close(fd2);
    return 0;
}