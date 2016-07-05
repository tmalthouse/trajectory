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

typedef uint64_t Time;

typedef double(*oneargfunc)(double a);

#define lambda(return_type, function_body) ({ \
      return_type lambda__fn__ function_body \
          lambda__fn__; \
})


#endif /* types_h */
