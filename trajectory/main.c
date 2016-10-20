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
    // insert code here...
    start_logger();
    //FILE *system = fopen("/Users/Thomas/Desktop/mercury.cfg", "r");
    
    SolarSystem sys = load_and_calculate_system("/Users/Thomas/Documents/XCode/trajectory/trajectory/SolarSystem.sqlite3");
    
    rungame(sys);
    //test();
    printf("Hello, World!\n");
    return 0;
}

void test()
{
    Body sun = {.name = "Sun!", .mass = 2e30};
    
    Body earth = {.name = "Earth!", .mass = 5.97237e24, .orbit = {.sma = 149598023000, .ecc = 0.0167086, .mna = 0.02413790317, .inc = 0.12487831, .ape = 1.993288179, .lan = -0.1965, .parent = &sun, .epoch = 0}};
    
    sun.mu = calculate_mu(sun);
    earth.mu = calculate_mu(earth);
    calculate_state_vectors(&earth, 0);
    
    printf("hi");
    for(;;);///Go into loop so vars can be inspected in debugger.
}
