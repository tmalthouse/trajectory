//
//  body.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef body_h
#define body_h

#include <stdio.h>
#include "vector3d.h"
#include "types.h"

typedef struct {
    uint64_t id;
    
    double sma;
    double ecc;
    double inc;
    double ape;
    double lan;
    double mna;
    Time epoch;
    
    struct Body *parent;
} Orbit;


typedef struct Body {
    char name[64];
    Orbit orbit;
    double mass;
    Vector3d pos;
    Vector3d vel;
    double mu;
} Body;

double newton_raphson_iterate(oneargfunc f, oneargfunc fderiv, double guess, uint8_t iterations);

#endif /* body_h */
