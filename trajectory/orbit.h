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
#include "types.h"

struct CBody;//Incomplete type to avoid circular deps

typedef struct {
    float80_t sma;
    float80_t ecc;
    float80_t inc;
    float80_t lpe;
    float80_t lan;
    float80_t mna;
    float80_t eph;
    struct CBody *parent;
} Orbit;

#endif /* orbit_h */
