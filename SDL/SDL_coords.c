//
//  SDL_coords.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "SDL_coords.h"
#include <tgmath.h>
#include "SDL_main.h"
#include "../vector3d.h"
#include "../debug.h"

static Vector2d top_right, bottom_left, midpoint;
static Vector2d screensize;
static double ratio;

static double calculate_ratio(Vector2d top_right, Vector2d bottom_left)
{
    double xratio = screensize.x/(bottom_left.x-top_right.x);
    double yratio = screensize.y/(top_right.y-bottom_left.y);
    
    double ratio = (fabs(xratio) < fabs(yratio))? xratio : yratio;//Set the ratio to the smaller of the two.
    dblogger("Ratio is %f\n", ratio);
    return ratio;
}

void set_screensize(Vector2d size)
{
    screensize = size;
}

void set_minmax_coords(Vector2d tr, Vector2d bl)
{
    top_right = tr;
    bottom_left = bl;
    ratio = calculate_ratio(bl, tr);
    midpoint = (Vector2d){(tr.x+bl.x)/2, (tr.y+bl.y)/2};
}

Vector2d calculate_screencoord(Vector3d spacecoord)
{
    double xcoord = (spacecoord.x-midpoint.x)*ratio + (screensize.x/2);
    double ycoord = (midpoint.y-spacecoord.y)*ratio + (screensize.y/2);
    
    return (Vector2d){xcoord, ycoord};
}