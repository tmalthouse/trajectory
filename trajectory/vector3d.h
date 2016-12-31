//
//  vector3d.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef vector3d_h
#define vector3d_h

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "types.h"

#if __has_extension(attribute_ext_vector_type)
#define OPENCL_VECTORS
#endif

// Vector3d is a 3-component vector of doubles, representing an (xyz) vector quantity
// God bless clang and OpenCL vectors. Drop in replacement for the struct
// version.
// A 4-component vector is used so the size is a power of 2, which is faster.
// We just ignore the last (w) term.
#ifdef OPENCL_VECTORS
typedef double Vector3d __attribute__((ext_vector_type(3)));
#else
typedef struct {
  double x;
  double y;
  double z;
} Vector3d;
#endif

// A vector with no magnitude and undefined direction. Useful for indicating
// uninitialized vectors.
#define V3D_0_VECTOR ((Vector3d){0, 0, 0})

// A unit vector pointing to +x
#define V3D_I_VECTOR ((Vector3d){1, 0, 0})

// A unit vector pointing to +y
#define V3D_J_VECTOR ((Vector3d){0, 1, 0})

// A unit vector pointing to +z
#define V3D_K_VECTOR ((Vector3d){0, 0, 1})

// Checks two vectors for equality. Returns true if all elements match, false
// if there is any nonmatch.
// @a the first vector to be tested.
// @b the second vector to be tested
// @return true if the vectors are equal, false otherwise
bool v3d_equal(Vector3d a, Vector3d b);

// Checks two vectors for equality, accountin for fp stuff. Not accurate for
// small vectors.
// @a the first vector to be tested.
// @b the second vector to be tested
// @return true if the vectors are (approximately) equal, false otherwise
bool v3d_fp_eq(Vector3d a, Vector3d b);

// Returns the magnitude (absolute value) of a vector.
// @v the vector whose absolute value is returned
// @return the absolute value of v
double v3d_abs(Vector3d v);

// Returns the absolute distance between two vector heads. Especially useful
// for coordinates
// @a the first vector
// @b the second vector
// @return the absolute difference between a and b
double v3d_absdist(Vector3d a, Vector3d b);

// Returns the dot (or scalar) product of two vectors.
// @a the first vector
// @b the second vector
// @return the dot product of a and b
double v3d_dotprod(Vector3d a, Vector3d b);

// Returns the angle between two vectors in radians. Normalized to fall between
// -π and π.
// @a the first vector
// @b the second vector
// @return the angle between a and b. Between -π and π.
double v3d_vector_angle(Vector3d a, Vector3d b);

// Returns the vector that, when added to b, gives a.
// @a the first vector
// @b the second vector
// @return the vector difference between the two
Vector3d v3d_vdiff(Vector3d a, Vector3d b);

// Returns the vector sum of two vectors.
// @a the first vector
// @b the second vector
// @return the vector sum of the two
Vector3d v3d_vsum(Vector3d a, Vector3d b);

// Returns vector a, multiplied by scalar factor f
// @a the vector to multiply
// @f the factor to multiply a by
// @return a*f
Vector3d v3d_fmult(Vector3d a, double f);

// Returns the unit vector (vector with magnitude 1) of a vector
// @a the vector
// @return the unit vector pointing in the same direction as a
Vector3d v3d_unit_vector(Vector3d a);

// Returns the cross (or vector) product of a and b, assuming a rh coordiate
// system.
// @a the first vector
// @b the second vector
// @return aXb
Vector3d v3d_xprod(Vector3d a, Vector3d b);

// Returns the sum of an array of vectors.
// @vectors a pointer to an array of vectors
// @count the length of that array
// @return the vector total of that array
Vector3d v3d_asum(Vector3d *vectors, uint64_t count);

// Returns the sum of n vectors. Useful for when you don't want to create an
// array for v3d_asum. Make sure count is equal to the number of vectors, or
// bad bad things happen.
#define v3d_nsum(count, ...) (v3d_asum((Vector3d[]){__VA_ARGS__}, count))

#endif /* vector3d_h */
