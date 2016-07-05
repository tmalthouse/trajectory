//
//  orbit.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "orbit.h"
#include "vector3d.h"
#include "types.h"

/// The newton_raphson_iterator function (unsuprisingly) implements the Newton-Raphson iteration.
/// Given a pointer to a function (The GCC inline functions are really helpful here), a pointer to its derivative function, an initial guess, and a
/// number of iterations to do, it returns an approximation of the zero of f. Iterations is capped at 255 because there's no need for more.
double newton_raphson_iterator(oneargfunc f, oneargfunc fderiv, double guess, uint8_t iterations)
{
    if (iterations == 0) {
        return guess;
    }

    double newguess = guess - (((*f)(guess))/((*fderiv)(guess)));
    return newton_raphson_iterator(f, fderiv, newguess, iterations-1);
}
