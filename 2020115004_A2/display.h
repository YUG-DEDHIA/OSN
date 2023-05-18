#ifndef __DISPLAY_H__
#define __DISPLAY_H__

int display_check(char* token,int* found,char * start_dir);
void ImplementDisplay(char* start_dir, int flagd, int flagh, char* s);
void ImplementDisplayWithFile(char* start_dir, int flagd, int flagf, char* file, char* s);

#endif