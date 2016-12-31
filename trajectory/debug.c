//
//  debug.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "vector3d.h"

static const char *log_path = "/Users/Thomas/Desktop/trajectory.log";
static FILE *logfile;

void logger(char *fmt, ...);

void start_logger() {
  logfile = fopen(log_path, "w");

  if (!logfile) {
    perror("File error");
    exit(EXIT_FAILURE);
  }
// logfile = stdout;

#ifndef OPENCL_VECTORS
  logger(
      "clang OPENCL vectors not supported/enabled. Falling back to software "
      "vectors. Performance may be affected.");
#else
  logger("Using clang OPENCL harware accelerated vectors.");
#endif
}

void logger(char *fmt, ...) {
  if (logfile) {
    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);
    putc('\n', logfile);
    fflush(logfile);
    return;
  }

  // Outputting to stderr probably isn't all that useful, but it's the best we
  // can do without a logfile
  fprintf(stderr,
          "logfile is NULL! Did you forget to initialize it or prematurely "
          "close it?");
}

void dblogger(char *fmt, ...) {
#ifdef LIZARD_KING  // We want debug info, but this is too much
  va_list args;
  va_start(args, fmt);
  vfprintf(logfile, fmt, args);
  va_end(args);
#endif
}

void end_logger() {
  fclose(logfile);
  logfile = NULL;
}
