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
#include <stdbool.h>
#include <stdarg.h>
#include "types.h"

typedef struct V3{
    double x;
    double y;
    double z;
} Vector3d;

#define V3D_0_VECTOR ((Vector3d){0,0,0})
#define V3D_I_VECTOR ((Vector3d){1,0,0})
#define V3D_J_VECTOR ((Vector3d){0,1,0})
#define V3D_K_VECTOR ((Vector3d){0,0,1})

bool v3d_equal(Vector3d a, Vector3d b);
double v3d_abs(Vector3d v);
double v3d_absdist(Vector3d a, Vector3d b);
double v3d_dotprod(Vector3d a, Vector3d b);
double v3d_vector_angle(Vector3d a, Vector3d b);
Vector3d v3d_vdiff(Vector3d a, Vector3d b);
Vector3d v3d_vsum(Vector3d a, Vector3d b);
Vector3d v3d_fmult(Vector3d a, double f);
Vector3d v3d_unit_vector(Vector3d a);
Vector3d v3d_xprod(Vector3d a, Vector3d b);
Vector3d v3d_asum(Vector3d *vectors, uint64_t count);
Vector3d v3d_nsum(int count, ...);



#endif /* vector3d_h */
