//
//  vector3d.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "vector3d.h"
#include <tgmath.h>

#define SQ(x) (x*x)

inline bool v3d_equals(Vector3d a, Vector3d b)
{
    if (a.x==b.x && a.y==b.y && a.z==b.z) {
        return true;
    }
    return false;
}

inline double v3d_abs(Vector3d v)
{
    return v3d_absdist(v, (Vector3d){0,0,0});
}

inline double v3d_absdist(Vector3d a, Vector3d b)
{
    Vector3d diff = v3d_vdiff(a, b);
    return sqrt(SQ(diff.x)+SQ(diff.y)+SQ(diff.z));
}

inline Vector3d v3d_vdiff(Vector3d a, Vector3d b)
{
    return (Vector3d){a.x-b.x, a.y-b.y, a.z-b.z};
}

inline Vector3d v3d_vsum(Vector3d a, Vector3d b)
{
    return (Vector3d){a.x+b.x, a.y+b.y, a.z+b.z};
}

inline Vector3d v3d_fmult(Vector3d a, double f)
{
    return (Vector3d){a.x*f, a.y*f, a.z*f};
}

inline double v3d_dotprod(Vector3d a, Vector3d b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vector3d v3d_xprod(Vector3d a, Vector3d b)
{
    return (Vector3d){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.y, a.x*b.y - a.y*b.x};
}

inline double v3d_vector_angle(Vector3d a, Vector3d b)
{
    return acos(v3d_dotprod(a, b)/(v3d_abs(a)*v3d_abs(b)));
}

inline Vector3d v3d_unit_vector(Vector3d a)
{
    double abs = v3d_abs(a);
    
    //Divide each quantity by the magnitude
    return v3d_fmult(a, 1/abs);
}

Vector3d v3d_asum(Vector3d *vectors, uint64_t count)
{
    if (count == 0) {
        return V3D_0_VECTOR;
    }
    return v3d_vsum(vectors[0], v3d_asum(vectors+1, count-1));
}

Vector3d v3d_nsum(int count, ...)
{
    if (count == 0) {
        return V3D_0_VECTOR;
    }
    
    va_list args;
    va_start(args, count);
    Vector3d sum = V3D_0_VECTOR;
    
    for (int i=0; i<count; i++) {
        sum = v3d_vsum(sum, va_arg(args, Vector3d));
    }
    va_end(args);
    return sum;
}
