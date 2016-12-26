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

typedef struct {
  double x;
  double y;
} Vector2d;

typedef struct {
  Vector2d a;
  Vector2d b;
} Vector2dPair;

double v2d_absdist(Vector2d a, Vector2d b);

#endif /* vector2d_h */
