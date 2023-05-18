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
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#include "prompt.h"
#include "InputManage.h"
#include "InBuilt.h"
#include "LS.h"
#include "system.h"
#include "repeat.h"
#include "pinfo.h"
#include "history.h"
#include "IO.h"
#include "pipe.h"
#include "jobs.h"
#include "sig.h"
#include "bg_fg.h"
#include "handlers.h"
#include "replay.h"

//structs
struct background_process{
    pid_t pid;
    char name[250];
    char status[100];
    int job_count;
};



//Global variables
struct background_process bg_process[10000];
int bg_count;
int active_process;
int job_count;
struct background_process fg_process[10000];
int fg_count;
int x;

#endif
