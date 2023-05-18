#ifndef __INPUTMANAGE_H__
#define __INPUTMANAGE_H__

int ManageInput(char* ch);
int TokeizeWithSpace(char *token, char args[][100],int *bg);
int ChangStrToInt(char*ch);
char* CreateToken(char** arg,int n);
#endif