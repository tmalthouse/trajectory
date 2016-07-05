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



double body_g_acc(Body a, Body s)
{
    double dist = v3d_absdist(a.pos, s.pos);
    return (BIG_G * a.mass)/(dist*dist);
}

double parent_mu(Orbit o)
{
    if (o.parent == NULL) {
        return 0.0;
    }
    //The cast is a way to get rudimentary inheritance in C--mass is a property of Body, and CBody is a "superclass" of body that can be casted to Body (or the basic body can be accessed via CBody.body.
    return BIG_G * ((Body*)(o.parent))->mass;
}


Vector3d ang_mom_h(Vector3d r, Vector3d v)
{
    return v3d_xprod(r, v);
}

Vector3d node_vector(Vector3d h)
{
    return v3d_xprod(V3D_K_VECTOR, h);
}

Vector3d eccentricity_vector(double parent_mu, Vector3d r, Vector3d v)
{
    double inv_mu = (1/parent_mu);
    double v_sq = pow(v3d_abs(v), 2);

    Vector3d r_adj = v3d_fmult(r, v_sq - (parent_mu/v3d_abs(r)));

    return v3d_fmult(v3d_vdiff(r_adj,
                               v3d_fmult(v, v3d_dotprod(r, v))),
                     inv_mu);
}

double orb_parameter(Vector3d h, double parent_mu)
{
    double abs_h = v3d_abs(h);

    return (abs_h*abs_h)/parent_mu;
}

double orb_eccentricity(Vector3d e_vector)
{
    return v3d_abs(e_vector);
}

double orb_inclination(Vector3d h_vector)
{
    return acos(h_vector.z / v3d_abs(h_vector));
}

double orb_long_asc_node(Vector3d node_vector)
{
    double angle = acos(node_vector.x / v3d_abs(node_vector));

    if (node_vector.y<0) {
        angle *= -1;
    }

    return angle;
}

double arg_of_periapsis(Vector3d node_vector, Vector3d ecc_vector)
{
    double angle = acos(v3d_dotprod(node_vector, ecc_vector) / (v3d_abs(node_vector)*v3d_abs(ecc_vector)));

    if (ecc_vector.z<0) {
        angle *= -1;
    }

    return angle;
}

double true_anomaly(Vector3d ecc_vector, Vector3d pos, Vector3d vel)
{
    double angle = acos(v3d_dotprod(ecc_vector, pos)/(v3d_abs(ecc_vector)*v3d_abs(pos)));

    if(v3d_dotprod(pos, vel)<0) {
        angle *= -1;
    }

    return angle;
}

double ecc_anomaly(double ecc, double true_ano)
{
    double num = sqrt(1-ecc*ecc)*sin(true_ano);
    double den = 1 + ecc*cos(true_ano);

    return atan2(num, den);
}

double mean_anomaly(double ecc_ano, double ecc)
{
    return ecc_ano - ecc * sin(ecc_ano);
}


double mean_ano_to_ecc_ano(double mean_ano, double ecc)
{
    oneargfunc f = lambda (double, (double ecc_ano) {return (ecc_ano - ecc*sin(ecc_ano) - mean_ano);});
    oneargfunc f_deriv = lambda (double, (double ecc_ano) {return (1-ecc*cos(ecc_ano));});

    return newton_raphson_iterate(f, f_deriv, mean_ano, 16);
}

double mean_ano_at_t(Orbit o, Time t)
{
    double raw_ano = o.mna + (t-o.epoch) + sqrt(o.parent->mu/pow(o.sma, 3));
    return fmod(raw_ano, PI*2);
}

double true_ano_from_ecc_ano(double ecc_ano, double ecc)
{
    double num = sqrt(1+ecc)*sin(ecc_ano/2);
    double den = sqrt(1-ecc)*cos(ecc_ano/2);
    return 2*atan2(num,den);
}

double orbital_height_from_ecc_ano(Orbit o, double ecc_ano)
{
    return o.sma*(1-o.ecc*cos(ecc_ano));
}
