//
//  saveload.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/18/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "saveload.h"
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include "body.h"
#include "debug.h"

Orbit load_orbit(config_setting_t *orbit, Body *sys, uint64_t len)
{
    Orbit result;
    config_setting_lookup_float(orbit, "sma", &result.sma);
    config_setting_lookup_float(orbit, "ecc", &result.ecc);
    config_setting_lookup_float(orbit, "inc", &result.inc);
    config_setting_lookup_float(orbit, "ape", &result.ape);
    config_setting_lookup_float(orbit, "lan", &result.lan);
    config_setting_lookup_float(orbit, "mna", &result.mna);
    config_setting_lookup_float(orbit, "epoch", &result.epoch);
    
    result.parent = NULL;
    const char *refbody = NULL;
    config_setting_lookup_string(orbit, "reference_body", &refbody);
    
    for (int i=0; i<len; i++) {
        if (!strncmp(sys[i].name, refbody, 64)) {
            result.parent = &sys[i];
            break;
        }
    }
    
    if (!result.parent) {
        logger("The parent of body %s is NULL. This is expected for the central body. If this body is not meant to be the central body, check the configuration file!\n", sys[len-1].name);
    }
    
    return result;
}

SolarSystem load_system(FILE *sys)
{
    config_t config;
    config_init(&config);
    
    if (!config_read(&config, sys)) {
        fprintf(stderr, "Config parsing error: %s at line %d in file %s\n", config_error_text(&config),
                                                                            config_error_line(&config),
                                                                            config_error_file(&config));
        goto load_err;
    }
    
    config_setting_t *setting;
    const char *str;
    
    if (config_lookup_string(&config, "name", &str)) {
        logger("Loading system %s\n", str);
    } else {
        logger("Loading unnamed system. Adding a name is recommended!");
    }
    
    setting = config_lookup(&config, "system");
    
    if (!setting) {
        logger("Couldn't find \"system\" variable in config file!");
        goto load_err;
    }
    
    int body_count = config_setting_length(setting);
    
    Time t;
    config_setting_lookup_float(setting, "time", &t);
    
    //Past this, all error jumps MUST go to settings_error, to ensure system is freed.
    Body *system = calloc(sizeof(Body), body_count);
    
    for (int i=0; i<body_count; i++) {
        config_setting_t *body = config_setting_get_elem(setting, i);
        
        const char *bodyname;
        double mass = 0.0;
        int32_t color = 0x0;
        
        if (!(config_setting_lookup_string(body, "name", &bodyname)
              && config_setting_lookup_float(body, "mass", &mass)
              && config_setting_lookup_int(body, "color", &color))) {
            logger("Body %d, name %s, is incomplete!\n", i, bodyname);
            goto setting_err;
        }
        
        strncpy(system[i].name, bodyname, 64);
        system[i].mass = mass;
        system[i].color = hex_to_color(color);
        
        config_setting_t *orbit_setting= config_setting_get_member(body, "orbit");
        system[i].orbit = load_orbit(orbit_setting, system, i);
    }
    
    config_destroy(&config);
    return (SolarSystem){.planets=system, .t=t, .count=body_count};
    
    
setting_err:
    free(system);
load_err:
    config_destroy(&config);
    return (SolarSystem){0};
}

void system_calculate_state_vectors(SolarSystem *s)
{
    for (int i=0; i<s->count; i++) {
        calculate_state_vectors(&s->planets[i], s->t);
    }
    return;
}


SolarSystem load_and_calculate_system(FILE *system_file)
{
    SolarSystem result = load_system(system_file);
    system_calculate_state_vectors(&result);
    return result;
}