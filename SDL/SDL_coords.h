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

void set_screensize(Vector2d size);
void set_minmax_coords(Vector2d tr, Vector2d bl);
void scale_display(double factor);
Vector2d calculate_screencoord(Vector3d spacecoord);

#endif /* SDL_coords_h */
