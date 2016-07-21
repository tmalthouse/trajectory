//
//  debug.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef debug_h
#define debug_h
#include <stdio.h>

static FILE *logfile;

void start_logger();
void logger(char *fmt, ...);
void dblogger(char *fmt, ...);
void end_logger();



#endif /* debug_h */
