//
//  spacecraft.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef spacecraft_h
#define spacecraft_h

#include <stdio.h>
#include "orbit.h"
#include "vector3d.h"
#include "body.h"
#include "types.h"

typedef struct {
    Body b;
} Spacecraft;


#endif /* spacecraft_h */
