//
//  SDL_coords.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef SDL_coords_h
#define SDL_coords_h

#include <stdio.h>
#include "../vector3d.h"
#include "../vector2d.h"
#include "../body.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


void set_screensize(Vector2d size);
void set_minmax_coords(Vector2d tr, Vector2d bl);
void scale_display(double factor);
void shift_display();
void change_shift(enum Direction dir);
Vector2d calculate_screencoord(Vector3d spacecoord);
void focus_body(Body *b);
int item_pointed_at(Vector2d *coords, uint64_t count, Vector2d mousecoord, float threshold);



#endif /* SDL_coords_h */
