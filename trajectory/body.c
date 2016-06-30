//
//  body.c
//  trajectory
//
//  Created by Thomas Malthouse on 6/28/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "body.h"
#include <tgmath.h>
#include "constants.h"
#include "vector3d.h"
#include "spacecraft.h"
#include "types.h"
#include "orbit.h"
#include "cbody.h"



float80_t body_g_acc(Body a, Body s)
{
    float80_t dist = v3d_absdist(a.pos, s.pos);
    return (BIG_G * a.mass)/(dist*dist);
}

float80_t parent_mu(Orbit o)
{
    if (o.parent == NULL) {
        return 0.0;
    }
    //The cast is a way to get rudimentary inheritance in C--mass is a property of Body, and CBody is a "superclass" of body that can be casted to Body (or the basic body can be accessed via CBody.body.
    return BIG_G * ((Body*)(o.parent))->mass;
}

float80_t mean_anomaly(Orbit *o, Time t)
{
    float80_t mu = o->parent->mu;
    
    return fmod(o->mna + t*sqrt(mu/pow(o->sma, 3)), (2*M_PI));
}

Vector3d ang_mom_h(Vector3d r, Vector3d v)
{
    return v3d_xprod(r, v);
}

Vector3d node_vector(Vector3d h)
{
    return v3d_xprod(V3D_K_VECTOR, h);
}

Vector3d eccentricity_vector(float80_t parent_mu, Vector3d r, Vector3d v)
{
    float80_t inv_mu = (1/parent_mu);
    float80_t v_sq = pow(v3d_abs(v), 2);
    
    Vector3d r_adj = v3d_fmult(r, v_sq - (parent_mu/v3d_abs(r)));
    
    return v3d_fmult(v3d_vdiff(r_adj,
                               v3d_fmult(v, v3d_dotprod(r, v))),
                     inv_mu);
}

float80_t orb_parameter(Vector3d h, float80_t parent_mu)
{
    float80_t abs_h = v3d_abs(h);
    
    return (abs_h*abs_h)/parent_mu;
}

float80_t orb_eccentricity(Vector3d e_vector)
{
    return v3d_abs(e_vector);
}

float80_t orb_inclination(Vector3d h_vector)
{
    return acos(h_vector.z / v3d_abs(h_vector));
}

float80_t orb_long_asc_node(Vector3d node_vector)
{
    float80_t angle = acos(node_vector.x / v3d_abs(node_vector));
    
    if (node_vector.y<0) {
        angle *= -1;
    }
    
    return angle;
}

float80_t arg_of_periapsis(Vector3d node_vector, Vector3d ecc_vector)
{
    float80_t angle = acos(v3d_dotprod(node_vector, ecc_vector) / (v3d_abs(node_vector)*v3d_abs(ecc_vector)));
    
    if (ecc_vector.z<0) {
        angle *= -1;
    }
    
    return angle;
}

float80_t true_anomaly(Vector3d ecc_vector, Vector3d pos, Vector3d vel)
{
    float80_t angle = acos(v3d_dotprod(ecc_vector, pos)/(v3d_abs(ecc_vector)*v3d_abs(pos)));
    
    if(v3d_dotprod(pos, vel)<0) {
        angle *= -1;
    }
    
    return angle;
}






