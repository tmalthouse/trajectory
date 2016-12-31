//
//  body.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "body.h"
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include "constants.h"
#include "debug.h"
#include "types.h"
#include "vector3d.h"

// Returns the g-force between the 2 given bodies.
Vector3d body_gforce(StateVector a, StateVector b) {
  double abs_dist = v3d_absdist(a.pos, b.pos);
  Vector3d unit_vect = v3d_unit_vector(v3d_vdiff(a.pos, b.pos));

  return v3d_fmult(unit_vect,
                   (BIG_G * a.mass * b.mass) / (abs_dist * abs_dist));
}

Vector3d net_gforce(Vector3d *forcetable_ptr, uint64_t focus, uint64_t count) {
  Vector3d(*forcetable)[count] = (Vector3d(*)[count])forcetable_ptr;

  Vector3d total = V3D_0_VECTOR;
  for (uint32_t i = 0; i < count; i++) {
    if (i == focus) continue;

    total = v3d_vsum(total, forcetable[focus][i]);
  }

  return total;
}

double calculate_mu(Body b) { return BIG_G * b.mass; }

double parent_mu(Orbit o) {
  if (o.parent == NULL) {
    return 0.0;
  }
  return calculate_mu(*o.parent);
}

Vector3d ang_mom_h(Vector3d r, Vector3d v) { return v3d_xprod(r, v); }

Vector3d node_vector(Vector3d h) { return v3d_xprod(V3D_K_VECTOR, h); }

Vector3d eccentricity_vector(double parent_mu, Vector3d r, Vector3d v,
                             Vector3d h) {
  double inv_mu = 1 / parent_mu;
  double abs_pos = v3d_abs(r);

  Vector3d vel_cross_mom = v3d_xprod(v, h);
  Vector3d vcm_div_mu = v3d_fmult(vel_cross_mom, inv_mu);

  Vector3d pos_div_abs = v3d_fmult(r, 1 / abs_pos);

  return v3d_vdiff(vcm_div_mu, pos_div_abs);
}

double orb_parameter(Vector3d h, double parent_mu) {
  double abs_h = v3d_abs(h);

  return (abs_h * abs_h) / parent_mu;
}

double orb_eccentricity(Vector3d e_vector) { return v3d_abs(e_vector); }

double orb_inclination(Vector3d h_vector) {
  return acos(h_vector.z / v3d_abs(h_vector));
}

double orb_long_asc_node(Vector3d node_vector) {
  double angle = acos(node_vector.x / v3d_abs(node_vector));

  if (node_vector.y < 0) {
    angle *= -1;
  }

  return angle;
}

double arg_of_periapsis(Vector3d node_vector, Vector3d ecc_vector) {
  double angle = acos(v3d_dotprod(node_vector, ecc_vector) /
                      (v3d_abs(node_vector) * v3d_abs(ecc_vector)));

  if (ecc_vector.z < 0) {
    angle *= -1;
  }

  return angle;
}

double true_anomaly(Vector3d ecc_vector, Vector3d pos, Vector3d vel) {
  double angle =
      acos(v3d_dotprod(ecc_vector, pos) / (v3d_abs(ecc_vector) * v3d_abs(pos)));

  if (v3d_dotprod(pos, vel) < 0) {
    angle *= -1;
  }

  return angle;
}

double ecc_anomaly(double ecc, double true_ano) {
  double num = sqrt(1 - ecc * ecc) * sin(true_ano);
  double den = 1 + ecc * cos(true_ano);

  return atan2(num, den);
}

double mean_anomaly(double ecc_ano, double ecc) {
  return ecc_ano - ecc * sin(ecc_ano);
}

/* Helper functions for ecc_ano_from_mean_ano*/
static double ecc = 0;
static double mean_ano = 0;

static double ecc_ano_func(double ecc_ano) {
  return (ecc_ano - ecc * sin(ecc_ano) - mean_ano);
}

static double ecc_ano_deriv_func(double ecc_ano) {
  return (1 - ecc * cos(ecc_ano));
}

double ecc_ano_from_mean_ano(double mean_ano_param, double ecc_param) {
  ecc = ecc_param;
  mean_ano = mean_ano_param;
  return newton_raphson_iterate(ecc_ano_func, ecc_ano_deriv_func, mean_ano, 16);
}
/*Now back to your regularly scheduled programming...*/

double mean_ano_at_t(Orbit o, Time t) {
  double raw_ano = o.mna + (t - o.epoch) + sqrt(o.parent->mu / pow(o.sma, 3));
  return fmod(raw_ano, PI * 2);
}

double true_ano_from_ecc_ano(double ecc_ano, double eccentricity) {
  double num = sqrt(1 + eccentricity) * sin(ecc_ano / 2);
  double den = sqrt(1 - eccentricity) * cos(ecc_ano / 2);
  return 2 * atan2(num, den);
}

double orbital_height_from_ecc_ano(Orbit o, double ecc_ano) {
  return o.sma * (1 - o.ecc * cos(ecc_ano));
}

Vector3d bodycentric_position(double orb_hgt, double true_ano) {
  Vector3d pos;
  pos.x = orb_hgt * cos(true_ano);
  pos.y = orb_hgt * sin(true_ano);
  pos.z = 0;
  return pos;
}

Vector3d bodycentric_velocity(Orbit o, double orb_hgt, double ecc_ano) {
  Vector3d vel;

  double factor = sqrt(o.parent->mu * o.sma) / orb_hgt;
  vel.x = factor * -1 * sin(ecc_ano);
  vel.y = factor * sqrt(1 - o.ecc * o.ecc) * cos(ecc_ano);
  vel.z = 0;
  return vel;
}

Vector3d bodycentric_to_cartesian(Orbit o, Vector3d vect) {
  // This uses some kind of rotation matrix I found on the internet!
  Vector3d prod;

  prod.x =
      vect.x *
          (cos(o.ape) * cos(o.lan) - sin(o.ape) * cos(o.inc) * sin(o.lan)) -
      vect.y * (sin(o.ape) * cos(o.lan) + cos(o.ape) * cos(o.inc) * sin(o.lan));

  prod.y =
      vect.x *
          (cos(o.ape) * sin(o.lan) + sin(o.ape) * cos(o.inc) * cos(o.lan)) +
      vect.y * (cos(o.ape) * cos(o.inc) * cos(o.lan) - sin(o.ape) * sin(o.lan));

  prod.z =
      vect.x * (sin(o.ape) * sin(o.inc)) + vect.y * (cos(o.ape) * sin(o.inc));

  return prod;
}

void calculate_state_vectors(Body *b, Time t) {
  // If the body is root, pos and vel are 0.
  if (b->orbit.parent == NULL) {
    b->pos = V3D_0_VECTOR;
    b->vel = V3D_0_VECTOR;
    return;
  }

  double ecc_anomaly =
      ecc_ano_from_mean_ano(mean_ano_at_t(b->orbit, t), b->orbit.ecc);
  double true_anomaly = true_ano_from_ecc_ano(ecc_anomaly, b->orbit.ecc);
  double altitude = orbital_height_from_ecc_ano(b->orbit, ecc_anomaly);

  Vector3d b_pos = bodycentric_position(altitude, true_anomaly);
  Vector3d b_vel = bodycentric_velocity(b->orbit, altitude, ecc_anomaly);

  // Add the calculated position relative to the parent to the parent's absolute
  // position.
  b->pos =
      v3d_vsum(bodycentric_to_cartesian(b->orbit, b_pos), b->orbit.parent->pos);
  // And the same with the velocity.
  b->vel =
      v3d_vsum(bodycentric_to_cartesian(b->orbit, b_vel), b->orbit.parent->vel);

  return;
}

double orb_sma(Vector3d r, Vector3d v, double parent_mu) {
  return 1 / ((2 / v3d_abs(r)) - (pow(v3d_abs(v), 2) / parent_mu));
}

void calculate_orbit_params(Body *b) {
  Vector3d pos, vel;

  if (b->orbit.parent == NULL) {
    // If it's the root body, there are no real orbital params to speak of.
    b->orbit = (Orbit){0};
    return;
  } else {
    pos = v3d_vdiff(b->pos, b->orbit.parent->pos);
    vel = v3d_vdiff(b->vel, b->orbit.parent->vel);
  }

  Vector3d h = ang_mom_h(pos, vel);
  Vector3d e_vect = eccentricity_vector(b->orbit.parent->mu, pos, vel, h);

  Vector3d n_vect = node_vector(h);
  double t_ano = true_anomaly(e_vect, pos, vel);

  double inc = orb_inclination(h);
  double eccentr = orb_eccentricity(e_vect);
  double e_ano = ecc_anomaly(eccentr, t_ano);
  double lan = orb_long_asc_node(n_vect);
  double ape = arg_of_periapsis(n_vect, e_vect);
  double m_ano = mean_anomaly(e_ano, eccentr);
  double sma = orb_sma(pos, vel, b->orbit.parent->mu);

  b->orbit = (Orbit){.sma = sma,
                     .inc = inc,
                     .ecc = eccentr,
                     .lan = lan,
                     .ape = ape,
                     .mna = m_ano,
                     .epoch = 0};
}

double newton_raphson_iterate(unaryfunc f, unaryfunc fderiv, double guess,
                              uint8_t iterations) {
  if (iterations == 0) {
    return guess;
  }

  double newguess = guess - (((*f)(guess)) / ((*fderiv)(guess)));
  return newton_raphson_iterate(f, fderiv, newguess, iterations - 1);
}

void euler_step(StateVector *b, Vector3d acc, Vector3d vel, Time dt) {
  b->pos = v3d_vsum(b->pos, v3d_fmult(vel, dt));
  b->vel = v3d_vsum(b->vel, v3d_fmult(acc, dt));
}

static inline void fill_forcetable (StateVector *states,
                                   Vector3d *forcetable_ptr, uint64_t count) {
  Vector3d(*forcetable)[count] = (Vector3d(*)[count])forcetable_ptr;

  for (uint32_t i = 0; i < count; i++) {
    forcetable[i][i] = 0.0;
    for (uint32_t j = i + 1; j < count; j++) {
      Vector3d force = body_gforce(states[i], states[j]);
      forcetable[j][i] = force;
      forcetable[i][j] = v3d_fmult(force, -1);
    }
  }
}

typedef struct {
  Vector3d vel;
  Vector3d acc;
} RKStep;

static _Thread_local StateVector *usv_lsys = NULL;
static _Thread_local Vector3d *forcetable_usv = NULL;
static _Thread_local RKStep((*vel_acc_steps)[4]) = NULL;
static _Thread_local StateVector((*initial_states_usv)) = NULL;
static _Thread_local uint64_t usv_static_count = 0;

static inline void initialize_buffers_usv(uint64_t count, bool force_realloc) {
  if (usv_lsys == NULL || force_realloc) {
    logger(
        "Allocating update_state_vectors buffer. This should only happen on "
        "start.");
    free(usv_lsys);
    usv_lsys = calloc(count, sizeof(StateVector));
  }

  if (forcetable_usv == NULL || force_realloc) {
    logger("Allocating forcetable. This should only happen on start.");
    free(forcetable_usv);
    forcetable_usv = calloc(count * count, sizeof(Vector3d));
  }

  if (vel_acc_steps == NULL || force_realloc) {
    logger("Allocating step buffer. This should only happen on start.");
    free(vel_acc_steps);
    vel_acc_steps = calloc(count, sizeof(RKStep[4]));
  }

  if (initial_states_usv == NULL || force_realloc) {
    logger("Allocating state buffer. This should only happen on start.");
    free(initial_states_usv);
    initial_states_usv = calloc(count, sizeof(StateVector));
  }

  usv_static_count = count;
}

void update_state_vectors(Body *sys, uint64_t count, Time dt) {
  bool force_realloc = count == usv_static_count ? false : true;

  initialize_buffers_usv(count, force_realloc);

  size_t forcetable_size = count * count * sizeof(Vector3d);

  StateVector *lsys = usv_lsys;

  memset(forcetable_usv, 0, forcetable_size);

  Vector3d(*forcetable)[count] = (Vector3d(*)[count])forcetable_usv;

  StateVector *initial_states = initial_states_usv;

  // The commented code in this section is from the original implementation,
  // which calculated the bodies serially. The actions performed in this code
  // are performed on every body in the existing code, which reduces (by half)
  // the number of calculations required, and allows for vectorization should
  // the optimizer want it.

  // Vector3d init_pos = focus->pos;
  // Vector3d init_vel = focus->vel;

  for (uint32_t i = 0; i < count; i++) {
    lsys[i].pos = (initial_states[i].pos = sys[i].pos);
    lsys[i].vel = (initial_states[i].vel = sys[i].vel);
    lsys[i].mass = (initial_states[i].mass = sys[i].mass);
  }

  fill_forcetable(initial_states, (Vector3d *)forcetable, count);

  // Step one of our two simultaneous runge-kuttas.
  // Vector3d acc_1 = v3d_fmult(net_gforce(lsys, count, bodyid),
  // 1/(focus->mass)); // Aka dv/dt
  // Vector3d vel_1 = init_vel; // Aka dr/dt
  for (uint32_t i = 0; i < count; i++) {
    vel_acc_steps[i][0].acc =
        v3d_fmult(net_gforce((Vector3d *)forcetable, i, count),
                  1 / initial_states[i].mass);
    vel_acc_steps[i][0].vel = initial_states[i].vel;
  }

  // euler_step(focus, acc_1, vel_1, dt/2);
  for (uint32_t i = 0; i < count; i++) {
    euler_step(&lsys[i], vel_acc_steps[i][0].acc, vel_acc_steps[i][0].vel,
               dt / 2);
  }

  // Now step two: the second slopes
  // Vector3d acc_2 = v3d_fmult(net_gforce(lsys, count, bodyid),
  // 1/(focus->mass));
  // Vector3d vel_2 = focus->vel;

  // Reset it for the mini-euler
  // focus->pos = init_pos;
  // focus->vel = init_vel;

  fill_forcetable(lsys, (Vector3d *)forcetable, count);
  for (uint32_t i = 0; i < count; i++) {
    vel_acc_steps[i][1].acc =
        v3d_fmult(net_gforce((Vector3d *)forcetable, i, count),
                  1 / initial_states[i].mass);
    vel_acc_steps[i][1].vel = initial_states[i].vel;
    lsys[i].pos = initial_states[i].pos;
    lsys[i].vel = initial_states[i].vel;
  }

  // euler_step(focus, acc_2, vel_2, dt/2);

  for (uint32_t i = 0; i < count; i++) {
    euler_step(&lsys[i], vel_acc_steps[i][1].acc, vel_acc_steps[i][1].vel,
               dt / 2);
  }

  // Step 3!
  // Vector3d acc_3 = v3d_fmult(net_gforce(lsys, count, bodyid),
  // 1/(focus->mass));
  // Vector3d vel_3 = focus->vel;

  // Reset again!
  // focus->pos = init_pos;
  // focus->vel = init_vel;

  fill_forcetable(lsys, (Vector3d *)forcetable, count);
  for (uint32_t i = 0; i < count; i++) {
    vel_acc_steps[i][2].acc =
        v3d_fmult(net_gforce((Vector3d *)forcetable, i, count),
                  1 / initial_states[i].mass);
    vel_acc_steps[i][2].vel = initial_states[i].vel;
    lsys[i].pos = initial_states[i].pos;
    lsys[i].vel = initial_states[i].vel;
  }

  // euler_step(focus, acc_3, vel_3, dt);
  for (uint32_t i = 0; i < count; i++) {
    euler_step(&lsys[i], vel_acc_steps[i][2].acc, vel_acc_steps[i][2].vel, dt);
  }

  // One last step...
  // Vector3d acc_4 = v3d_fmult(net_gforce(lsys, count, bodyid),
  // 1/(focus->mass));
  // Vector3d vel_4 = focus->vel;
  fill_forcetable(lsys, (Vector3d *)forcetable, count);
  for (uint32_t i = 0; i < count; i++) {
    vel_acc_steps[i][3].acc =
        v3d_fmult(net_gforce((Vector3d *)forcetable, i, count),
                  1 / initial_states[i].mass);
    vel_acc_steps[i][3].vel = initial_states[i].vel;
    lsys[i].pos = initial_states[i].pos;
    lsys[i].vel = initial_states[i].vel;
  }

  // Vector3d v_combined = v3d_vsum(init_vel,
  //                               v3d_fmult(v3d_nsum(4, acc_1,
  //                                                  v3d_fmult(acc_2, 2),
  //                                                  v3d_fmult(acc_3, 2),
  //                                                  acc_4),
  //                                         dt/6));
  // Vector3d p_combined = v3d_vsum(init_pos,
  //                               v3d_fmult(v3d_nsum(4, vel_1,
  //                                                  v3d_fmult(vel_2, 2),
  //                                                  v3d_fmult(vel_3, 2),
  //                                                  vel_4),
  //                                          dt/6));

  for (uint32_t i = 0; i < count; i++) {
    Body *original = &sys[i];

    original->vel =
        v3d_vsum(initial_states[i].vel,
                 v3d_fmult(v3d_nsum(4, vel_acc_steps[i][0].acc,
                                    v3d_fmult(vel_acc_steps[i][1].acc, 2),
                                    v3d_fmult(vel_acc_steps[i][2].acc, 2),
                                    vel_acc_steps[i][3].acc),
                           dt / 6));

    original->pos =
        v3d_vsum(initial_states[i].pos,
                 v3d_fmult(v3d_nsum(4, vel_acc_steps[i][0].vel,
                                    v3d_fmult(vel_acc_steps[i][1].vel, 2),
                                    v3d_fmult(vel_acc_steps[i][2].vel, 2),
                                    vel_acc_steps[i][3].vel),
                           dt / 6));
  }

  return;
}

void system_update(Body *sys, uint64_t count, Time dt, Time *t) {
  update_state_vectors(sys, count, dt);
  *t += dt;
}

long double system_total_energy(Body *sys, uint64_t count) {
  long double total_e = 0;

  for (uint64_t i = 0; i < count; i++) {
    total_e += 0.5 * sys[i].mass * pow(v3d_abs(sys[i].vel), 2) / 1.0e12;
    if (isinf(total_e)) {
      logger(
          "total_e overflowed. This can happen with very high-mass objects. "
          "The returned evergy will not be accurate.");
      return HUGE_VALL;
    }
  }

  return total_e;
}

// Destructor for a thread that calls update_state_vectors anywhere. Note that
// this function is safe to call at any time--i.e. it doen't break internal
// state (the buffer will simply be reallocated) and will work even if the
// buffer has never been allocated (since free(NULL) is safe.)
void update_sv_thread_destructor(void *ptr) {
  free(usv_lsys);
  usv_lsys = NULL;
  free(forcetable_usv);
  forcetable_usv = NULL;
  free(initial_states_usv);
  initial_states_usv = NULL;
  free(vel_acc_steps);
  vel_acc_steps = NULL;
  usv_static_count = 0;
}

void print_body_info(Body b) {
  logger("For body %s:\n Mass=%f. SMA=%f. Pos={%f,%f,%f}.\n Vel={%f,%f,%f}\n",
         b.name, b.mass, b.orbit.sma, b.pos.x, b.pos.y, b.pos.z, b.vel.x,
         b.vel.y, b.vel.z);
}
