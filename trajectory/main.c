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

//const char *db =
//    "/Users/Thomas/Documents/xcode/trajectory/trajectory/SolarSystem.sqlite3";
const char *db = "/Users/Thomas/Desktop/cloud.db";

int main(int argc, const char *argv[]) {
  start_logger();

  SolarSystem sys = load_and_calculate_system(db);

  rungame(sys);

  store_system(sys, db);

  end_logger();
  return 0;
}
