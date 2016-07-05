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

#define lambda(return_type, function_body) \
__extension__({ \
return_type fn__lambda function_body \
nested_fn; \
})


#endif /* types_h */
