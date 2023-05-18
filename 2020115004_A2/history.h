#ifndef __HISTORY_H__
#define __HISTORY_H__

int get_num_of_lines(char* filename);
void read_history_from_file(char* filename,char history[][10000], int num);
void write_history_into_file(char* filename,char history[][10000],int num);

int check_history(char ** argv, int argc, int * found,char history[][10000],int c_h);
void calc_history(char history[][10000], int c_h, int x);
#endif