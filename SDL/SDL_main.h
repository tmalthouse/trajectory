//
//  SDL_main.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/13/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef SDL_main_h
#define SDL_main_h

#include <stdio.h>
#define PLANET_SIZE (2)

typedef struct {
    double x;
    double y;
} Vector2d;

typedef struct {
    Vector2d a;
    Vector2d b;
} Vector2dPair;

void rungame();

#endif /* SDL_main_h */
