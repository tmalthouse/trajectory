//
//  main.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include "body.h"
#include "SDL/SDL_main.h"

void test();

int main(int argc, const char * argv[]) {
    // insert code here...
    //rungame();
    test();
    printf("Hello, World!\n");
    return 0;
}

void test()
{
    Body sun = {.name = "Sun!", .mass = 2e30};
    
    Body earth = {.name = "Earth!", .mass = 5.97237e24, .orbit = {.sma = 149598023000, .ecc = 0.0167086, .mna = 0.02413790317, .inc = 0.12487831, .ape = 1.993288179, .lan = -0.1965, .parent = &sun, .epoch = 0}};
    
    calculate_state_vectors(&earth, 0);
    
    printf("hi");
}