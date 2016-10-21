//
//  main.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include "body.h"
#include "debug.h"
#include "SDL/SDL_main.h"
#include "saveload.h"


void test();

int main(int argc, const char * argv[]) {

    start_logger();
    
    SolarSystem sys = load_and_calculate_system("/Users/Thomas/Documents/xcode/trajectory/trajectory/SolarSystem.sqlite3");
    
    rungame(sys);

    return 0;
}
