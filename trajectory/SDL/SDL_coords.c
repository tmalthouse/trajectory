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
#include "../vector2d.h"
#include "../debug.h"
#include "../body.h"

static Vector2d top_right, bottom_left, midpoint;
static Vector2d screensize;
static double ratio, original_ratio;
static Body *focus;

static double dx, dy;

static double calculate_ratio(Vector2d tr, Vector2d bl)
{
    double xratio = screensize.x/(bl.x-tr.x);
    double yratio = screensize.y/(tr.y-bl.y);
    
    double lratio = (fabs(xratio) < fabs(yratio))? xratio : yratio;//Set the ratio to the smaller of the two.
    original_ratio = lratio;
    dblogger("Ratio is %f\n", lratio);
    return lratio;
}

void set_screensize(Vector2d size)
{
    screensize = size;
}

void scale_display(double factor)
{
    ratio *= factor;
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

void shift_display()
{
    /*
    double xdist = top_right.x-bottom_left.x;
    
    double ydist = top_right.y-bottom_left.y;
    
    printf("xdist: %f, ydist: %f\n", xdist, ydist);
    
    top_right.x += dx*xdist*(original_ratio/ratio);
    bottom_left.x += dx*xdist*(original_ratio/ratio);
    midpoint.x += dx*xdist*(original_ratio/ratio);
    
    top_right.y += dy*ydist*(original_ratio/ratio);
    bottom_left.y += dy*ydist*(original_ratio/ratio);
    midpoint.y += dy*ydist*(original_ratio/ratio);
    
    if (dx>0) {
        dx-=.02;
    } else if (dx<0) {
        dx+=.02;
    }
    
    if (dy>0) {
        dy-=.01;
    } else if (dy<0) {
        dy+=.01;
    }
    
    printf("dx: %f, dy: %f\n", dx, dy);
    
    if (fabs(dx) < 0.016) dx=0;
    if (fabs(dy) < 0.008) dy=0;
     */
    if (focus) {
        midpoint.x = focus->pos.x;
        midpoint.y = focus->pos.y;
    }
}

void focus_body(Body *b)
{
    focus = b;
}

void change_shift(enum Direction dir)
{
    switch (dir) {
        case LEFT:
            dx+=0.1;
            break;
        case RIGHT:
            dx-=0.1;
            break;
        case UP:
            dy-=0.05;
            break;
        case DOWN:
            dy+=0.05;
            break;
        default:
            break;
    }
}

int item_pointed_at(Vector2d *coords, uint64_t count, Vector2d mousecoord, float threshold)
{
    int closest_obj = -1;
    double closest_dist = threshold;
    for (uint32_t i=0; i<count; i++) {
        double dist = v2d_absdist(mousecoord, coords[i]);
        if (dist<closest_dist) {
            closest_obj = i;
            closest_dist = dist;
        }
    }
    return closest_obj;
}
