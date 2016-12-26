//
//  test_vector3d.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/19/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "test_vector3d.h"
#include <stdbool.h>
#include <tgmath.h>
#include "../vector3d.h"

static bool fp_eq(double a, double b) {
  return fabs(a - b) < 0.1 ? true : false;
}

void report(const char *fn, bool result) {
  printf("%s %s!\n", fn, result ? "✅ PASSED" : "❌ FAILED");
}

bool test_v3d_equals() {
  bool success = true;

  success *= v3d_equal((Vector3d){1, 1, 1}, (Vector3d){1, 1, 1});

  success *= !v3d_equal((Vector3d){1, 1, 1}, (Vector3d){1, 0, 1});

  report(__func__, success);
  return success;
}

bool test_v3d_abs() {
  bool success = true;

  success *= fp_eq(v3d_abs((Vector3d){3, 4, 0}), 5);

  success *= !fp_eq(v3d_abs((Vector3d){3, 4, 0}), 43804328);

  report(__func__, success);
  return success;
}

bool test_v3d_absdist() {
  bool success = true;

  success *= fp_eq(v3d_absdist((Vector3d){3, 4, 5}, (Vector3d){431, -532, 872}),
                   1105.52);

  report(__func__, success);
  return success;
}

bool test_v3d_dotprod() {
  bool success = true;

  success *=
      fp_eq(v3d_dotprod((Vector3d){3, 4, 5}, (Vector3d){431, -532, 872}), 3525);

  report(__func__, success);
  return success;
}

bool test_v3d_vector_angle() {
  bool success = true;

  success *=
      fp_eq(v3d_vector_angle((Vector3d){3, 4, 5}, (Vector3d){431, -532, 872}),
            1.1044301508169406);

  report(__func__, success);
  return success;
}

bool test_v3d_vdiff() {
  bool success = true;

  success *=
      v3d_fp_eq(v3d_vdiff((Vector3d){3, 4, 5}, (Vector3d){432, -532, 872}),
                (Vector3d){-429, 536, -867});

  report(__func__, success);
  return success;
}

bool test_v3d_vsum() {
  bool success = true;

  success *=
      v3d_fp_eq(v3d_vsum((Vector3d){3, 4, 5}, (Vector3d){432, -532, 872}),
                (Vector3d){435, -528, 877});

  report(__func__, success);
  return success;
}

bool test_v3d_fmult() {
  bool success = true;

  success *= v3d_fp_eq(v3d_fmult((Vector3d){3, 4, 5}, 871),
                       (Vector3d){2613, 3484, 4355});

  report(__func__, success);
  return success;
}

bool test_v3d_unit_vector() {
  bool success = true;

  success *= v3d_fp_eq(v3d_unit_vector((Vector3d){432, -532, 872}),
                       (Vector3d){0.389516, -0.479682, 0.786246});

  report(__func__, success);
  return success;
}

bool test_v3d_xprod() {
  bool success = true;

  success *=
      v3d_fp_eq(v3d_xprod((Vector3d){3, 4, 5}, (Vector3d){432, -532, 872}),
                (Vector3d){6148, -456, -3324});

  report(__func__, success);
  return success;
}
