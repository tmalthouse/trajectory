//
//  main.c
//  trajectory.testing
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include "test_vector3d.h"

typedef bool(*testfunc)();

testfunc v3d_tests[] = {test_v3d_equals,
    test_v3d_abs,
    test_v3d_absdist,
    test_v3d_dotprod,
    test_v3d_vector_angle,
    test_v3d_vdiff,
    test_v3d_vsum,
    test_v3d_fmult,
    test_v3d_unit_vector,
    test_v3d_xprod};

int main(int argc, const char * argv[]) {
    int test_count = (sizeof(v3d_tests)/sizeof(testfunc));
    int success_count = 0;

    for (int i=0; i<test_count; i++) {
        success_count += v3d_tests[i]();
    }
    printf("%d out of %d tests passed.\n", success_count, test_count);
    
    return 0;
}
