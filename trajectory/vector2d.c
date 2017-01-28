//
//  vector2d.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "vector2d.h"
#include <tgmath.h>

double v2d_absdist(Vector2d a, Vector2d b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
