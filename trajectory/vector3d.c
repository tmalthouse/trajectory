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

inline long double v3d_abs(Vector3d v)
{
    return v3d_absdist(v, (Vector3d){0,0,0});
}

inline long double v3d_absdist(Vector3d a, Vector3d b)
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

inline Vector3d v3d_fmult(Vector3d a, long double f)
{
    return (Vector3d){a.x*f, a.y*f, a.z*f};
}

inline Vector3d v3d_dotprod(Vector3d a, Vector3d b)
{
    return (Vector3d){a.x*b.x, a.y*b.y, a.z*b.z};
}

inline Vector3d v3d_xprod(Vector3d a, Vector3d b)
{
    return (Vector3d){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.y, a.x*b.y - a.y*b.x};
}