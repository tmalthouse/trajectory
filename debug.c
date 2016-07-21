//
//  debug.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



static const char log_path[] = "/Users/Thomas/Desktop/trajectory.log";
static FILE *logfile;


void start_logger()
{
    logfile = fopen(log_path, "w");
    
#ifdef __APPLE__
    system("system_profiler SPHardwareDataType > log_path");
#elif __LINUX__
    system("lscpu > log_path",);
#else
    logger("Unsupported OS! Things may not work right!");
#endif
    
    
    if (!logfile) {
        perror("File error");
        exit(EXIT_FAILURE);
    }
    //logfile = stdout;
}

void logger(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);
}

void dblogger(char *fmt, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);
#endif
}

void end_logger()
{
    fclose(logfile);
}