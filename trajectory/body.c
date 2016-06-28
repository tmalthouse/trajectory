//
//  body.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "body.h"
#include <tgmath.h>
#include "constants.h"
#include "vector3d.h"

long double body_gforce(Body a, Body b)
{
    long double dist = v3d_absdist(a.position, b.position);
    return (BIG_G * a.mass * b.mass)/(dist*dist);
}
