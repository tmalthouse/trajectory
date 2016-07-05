//
//  orbit.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef orbit_h
#define orbit_h

#include <stdio.h>
#include <stdint.h>
#include "types.h"

struct CBody;//Incomplete type to avoid circular deps

typedef struct {
    double sma;
    double ecc;
    double inc;
    double lpe;
    double lan;
    double mna;
    double epoch;
    struct CBody *parent;
} Orbit;

double newton_raphson_iterator(oneargfunc f, oneargfunc fderiv, double guess, uint8_t iterations);

#endif /* orbit_h */
