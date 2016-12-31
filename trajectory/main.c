//
//  main.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include "SDL/SDL_main.h"
#include "body.h"
#include "debug.h"
#include "saveload.h"

// const char *db =
//    "/Users/Thomas/Documents/xcode/trajectory/trajectory/SolarSystem.sqlite3";
const char *db = "/Users/Thomas/Desktop/cloud.db";

int main(int argc, const char *argv[]) {
  start_logger();
  
  const char *dbfile;
  if (argc == 2) {
    dbfile = argv[1];
  } else {
    logger("Using the default database at \"%s\". To specify a database, provide it as the first and only argument on the command line.", db);
    dbfile = db;
  }
  
  SolarSystem sys = load_and_calculate_system(dbfile);

  rungame(sys);

  store_system(sys, dbfile);

  end_logger();
  return 0;
}
