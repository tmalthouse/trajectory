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

struct Body;

typedef struct {
    long double sma;
    long double ecc;
    long double inc;
    long double lpe;
    long double lan;
    long double mna;
    long double eph;
    struct Body *parent;
} Orbit;

#endif /* orbit_h */
