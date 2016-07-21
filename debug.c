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

void log_system_info()
{
    FILE *sysinfo;
#ifdef __APPLE__
    printf("popen party!! On an apple!!\n");
    sysinfo = popen("system_profiler SPHardwareDataType", "r");
#elif __LINUX__
    printf("Penguin popen!\n");
    sysinfo = popen("lscpu", "r");
#endif
    char c;
    while ((c=fgetc(sysinfo)!=EOF)) {
        putc(c, stdout);
        fputc(c, logfile);
    }
    
    pclose(sysinfo);
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