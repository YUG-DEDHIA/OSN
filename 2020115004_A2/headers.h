#ifndef __HEADERS_H__
#define __HEADERS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>  //used ls -l
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>


#include "prompt.h"
#include "InputManage.h"
#include "InBuilt.h"
#include "LS.h"
#include "system.h"
#include "display.h"
#include "pinfo.h"
#include "history.h"

//Global variables
extern struct background_process bg_process[10000];
extern int bg_count;

#endif
