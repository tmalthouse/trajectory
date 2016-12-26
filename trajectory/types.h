//
//  types.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/29/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef types_h
#define types_h

#include <stdint.h>
#include <stdlib.h>

typedef double Time;

inline static double strtotime(char *str, char **endptr) {
  return strtod(str, endptr);
}

typedef double (*unaryfunc)(double a);

#endif /* types_h */
