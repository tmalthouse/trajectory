//
//  vector2d.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef vector2d_h
#define vector2d_h

#include <stdio.h>

// Vector2d is a vector of 2 doubles, representing an (x, y) quantity.
typedef struct {
  double x;
  double y;
} Vector2d;

// Vector2dPair is a vector of 2 2d vectors, mostly used to define rectangles.
typedef struct {
  Vector2d a;
  Vector2d b;
} Vector2dPair;

// v2d_absdist returns the absolute value of the distance between two Vector2d s
double v2d_absdist(Vector2d a, Vector2d b);

#endif /* vector2d_h */
