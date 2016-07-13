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

typedef struct {
    double x;
    double y;
    double z;
} Vector3d;

/// A vector with no magnitude and undefined direction. Useful for indicating uninitialized vectors.
#define V3D_0_VECTOR ((Vector3d){0,0,0})

/// A unit vector pointing to +x
#define V3D_I_VECTOR ((Vector3d){1,0,0})

/// A unit vector pointing to +y
#define V3D_J_VECTOR ((Vector3d){0,1,0})

/// A unit vector pointing to +z
#define V3D_K_VECTOR ((Vector3d){0,0,1})

/// Checks two vectors for equality. Returns true if all elements match, false if there is any nonmatch.
bool v3d_equal(Vector3d a, Vector3d b);

/// Returns the magnitude (absolute value) of a vector.
double v3d_abs(Vector3d v);

/// Returns the absolute distance between two vector heads. Especially useful for coordinates
double v3d_absdist(Vector3d a, Vector3d b);

/// Returns the dot (or scalar) product of two vectors.
double v3d_dotprod(Vector3d a, Vector3d b);

/// Returns the angle between two vectors in radians. Normalized to fall between -π and π.
double v3d_vector_angle(Vector3d a, Vector3d b);

/// Returns the vector that, when added to b, gives a.
Vector3d v3d_vdiff(Vector3d a, Vector3d b);

/// Returns the vector sum of two vectors.
Vector3d v3d_vsum(Vector3d a, Vector3d b);

/// Returns vector a, multiplied by scalar factor f
Vector3d v3d_fmult(Vector3d a, double f);

/// Returns the unit vector (vector with magnitude 1) pointing the direction of a
Vector3d v3d_unit_vector(Vector3d a);

/// Returns the cross (or vector) product of a and b, assuming a rh coordiate system.
Vector3d v3d_xprod(Vector3d a, Vector3d b);

/// Returns the sum of an array of vectors.
Vector3d v3d_asum(Vector3d *vectors, uint64_t count);

/// Returns the sum of n vectors. Useful for when you don't want to create an array for v3d_asum. Make sure count is equal to the number of vectors, or bad bad things happen.
Vector3d v3d_nsum(int count, ...);



#endif /* vector3d_h */
