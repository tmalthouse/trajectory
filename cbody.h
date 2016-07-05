//
//  cbody.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/29/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef cbody_h
#define cbody_h

#include <stdio.h>
#include "body.h"
#include "types.h"

typedef struct CBody {
    Body b;
    double mu;
} CBody;

#endif /* cbody_h */
