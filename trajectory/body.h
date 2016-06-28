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

typedef struct {
    char name[64];
    long double mass;
    Vector3d position;
    Vector3d velocity;
} Body;

long double body_gforce(Body a, Body b);


#endif /* body_h */
