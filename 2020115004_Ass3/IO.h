#ifndef __IO_H__
#define __IO_H__

// check presence of IO change
//gt_io=2 if change in stdin has occured and gt_io=3 if change has occured in stdout.
//gt_io=-1 for wrong input file path
int IOchangeCheck(char** a,int* c,char* token);

//checks and changes Stdin and returns previous file descriptor of stdin 
int ChangeInputStream(char *pathname);

//checks and changes Stdout with type=1 if overwrite and type = 2 if append and returns previous file descriptor of stdout.
void ChangeOutputStream(char *pathname,int type);

#endif