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
#include "orbit.h"
#include "types.h"

typedef struct Body {
    char name[64];
    Orbit orbit;
    double mass;
    Vector3d pos;
    Vector3d vel;
} Body;

#endif /* body_h */
