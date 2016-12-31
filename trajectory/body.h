//
//  body.h
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef body_h
#define body_h

#include <stdio.h>
#include "color.h"
#include "constants.h"
#include "types.h"
#include "vector3d.h"

typedef struct {
  double sma;
  double ecc;
  double inc;
  double ape;
  double lan;
  double mna;
  Time epoch;

  struct Body *parent;
} Orbit;

typedef struct {
  Vector3d pos;
  Vector3d vel;
  double mass;
} StateVector;

typedef struct Body {
  /* Persistent fields--These can and should be used across multiple timesteps
   */
  uint64_t id;
  char name[BODY_NAME_MAX_LEN];
  Orbit orbit;
  double mass;
  double mu;
  Color color;
  /* Varying fields--These can and will change rapidly, and should not be stored
   * anywhere but here. DO NOT write to these--just read them. Writing is only
   * to be done by the integrator function or the
   * orbital-element-calculator-thing*/
  Vector3d pos;
  Vector3d vel;

  uint16_t screensize;
} Body;


enum system_mode {
  MODE_PARAMS,
  MODE_STATES,
};

typedef struct {
  Body *planets;
  Time t;
  uint64_t count;
  enum system_mode mode;
} SolarSystem;

/** The newton_raphson_iterator function (unsuprisingly) implements the
   Newton-Raphson iteration.
    Given a pointer to a function (The GCC inline functions are really helpful
   here), a pointer to its derivative function, an initial guess, and a
    number of iterations to do, it returns an approximation of the zero of f.
   Iterations are capped at 255 because there's really no possible
    reason for more.
 */
double newton_raphson_iterate(unaryfunc f, unaryfunc fderiv, double guess,
                              uint8_t iterations);

// Returns the grav. parameter (mu) of the given body.
double calculate_mu(Body b);

// body_gforce returns a vector of the force exerted on body a by body b
Vector3d body_gforce(StateVector a, StateVector b);

// net_gforce returns a vector of the total gravitational force acting on
// sys[count] by all the other bodies in sys.
Vector3d net_gforce(Vector3d *forcetable, uint64_t body, uint64_t count);

// parent_mu calculates MG for the parent body and returns it. If parent==NULL
// (IE the body is the central body) the method returns 0.0/./
double parent_mu(Orbit o);

// ang_mom_h returns the angulare momentum vector (h) given state vectors.
Vector3d ang_mom_h(Vector3d r, Vector3d v);

// node_vector returns the orbital node vector n given the angular momentum
// vector.
Vector3d node_vector(Vector3d h);

// eccentricity vector calculates the e vector given the parent's mu and state
// vectors.
Vector3d eccentricity_vector(double parent_mu, Vector3d r, Vector3d v,
                             Vector3d h);

// orb_parameter returns the orbital parameter given the ang. momentum and
// parent's mu value. You'll probably never need to call this directly.
double orb_parameter(Vector3d h, double parent_mu);

// orb_eccentricity converts orb_eccentricity into a single floating value.
double orb_eccentricity(Vector3d e_vector);

// orb_inclination calculates the inclination given the angular momentum
// vector.
double orb_inclination(Vector3d h_vector);

// orb_long_asc_node calculates the longitude of the ascending node given the n
// vector.
double orb_long_asc_node(Vector3d node_vector);

// arg_of_periapsis uses the node and ecc vectors to calculate the argument of
// periapsis, one of Kepler's 5 elements.
double arg_of_periapsis(Vector3d node_vector, Vector3d ecc_vector);

// true_anomaly calculates the true anomaly (angle between the periapsis and
// current location based off the center body)
double true_anomaly(Vector3d ecc_vector, Vector3d pos, Vector3d vel);

// ecc_anomaly is like true anomaly, but the center of the angle is the center
// of the ellipse
double ecc_anomaly(double ecc, double true_ano);

// The mean anomaly is like the true anomaly, but the body's position if it
// were in a circular orbit with equal period
double mean_anomaly(double ecc_ano, double ecc);

// ecc_ano_from_mean_ano uses Newton's method to calculate the eccentric
// anomaly given the mean anomaly. This takes a bit longer than most functions
// here, so cache the result if it makes sense to.
double ecc_ano_from_mean_ano(double mean_ano, double ecc);

// mean_ano_at_t calculates the mean anomaly of an orbit at a time T after the
// epoch
double mean_ano_at_t(Orbit o, Time t);

// true_ano_from_ecc_ano does what it says on the box. It's very useful for
// updating the orbital elements.
double true_ano_from_ecc_ano(double ecc_ano, double ecc);

// Calculates the current orbital altitude  given an orbit and its eccentric
// anomaly. Useful for calculating the state vectors.
double orbital_height_from_ecc_ano(Orbit o, double ecc_ano);

// Returns the body's position in bodycentric coordinates.
Vector3d bodycentric_position(double orb_hgt, double true_ano);

// Returns the body's velocity in bodycentric coordinates
Vector3d bodycentric_velocity(Orbit o, double orb_hgt, double ecc_ano);

// Uses a simple rotation to convert bodycentric coordinates to Cartesian
// coordinates, given an orbit.
Vector3d bodycentric_to_cartesian(Orbit o, Vector3d vect);

// Given a body and its orbital elements, calculate its position and velocity
// at time t
void calculate_state_vectors(Body *b, Time t);

// Given a body and its state vectors, calculate the orbital elements. The
// opposite of calculate_state_vectors
void calculate_orbit_params(Body *b);

// Update a body's state vectors after a given timestep dt. Uses the very
// accurate Runge-Kutta 4 integration
void update_state_vectors(Body *sys, uint64_t count, Time dt);

// Updates every body in a system at time dt. Uses a constant timestep for all
// bodies, which is not optimal.
void system_update(Body *sys, uint64_t count, Time dt, Time *t);

// Returns the total energy of the system, in terajoules (1e12*J)
long double system_total_energy(Body *sys, uint64_t count);

void print_body_info(Body b);

#endif /* body_h */
