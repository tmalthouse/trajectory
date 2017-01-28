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

#if __has_extension(attribute_ext_vector_type)
#define OPENCL_VECTORS
#endif

// Vector2d is a vector of 2 doubles, representing an (x, y) quantity.
#ifdef OPENCL_VECTORS
typedef double Vector2d __attribute__((ext_vector_type(2))) ;
#else
typedef struct {
  double x;
  double y;
} Vector2d;
#endif

// Vector2dPair is a vector of 2 2d vectors, mostly used to define rectangles.
typedef struct {
  Vector2d a;
  Vector2d b;
} Vector2dPair;

// v2d_absdist returns the absolute value of the distance between two Vector2d s
double v2d_absdist(Vector2d a, Vector2d b);

#endif /* vector2d_h */
