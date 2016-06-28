//
//  vector3d.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef vector3d_h
#define vector3d_h

#include <stdio.h>

typedef struct {
    long double x;
    long double y;
    long double z;
} Vector3d;

long double v3d_abs(Vector3d v);
long double v3d_absdist(Vector3d a, Vector3d b);
Vector3d v3d_vdiff(Vector3d a, Vector3d b);
Vector3d v3d_vsum(Vector3d a, Vector3d b);
Vector3d v3d_fmult(Vector3d a, long double f);
Vector3d v3d_dotprod(Vector3d a, Vector3d b);
Vector3d v3d_xprod(Vector3d a, Vector3d b);

#endif /* vector3d_h */
