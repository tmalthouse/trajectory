//
//  saveload.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/18/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "saveload.h"
#include <stdlib.h>
#include <libconfig.h>
#include "body.h"


SolarSystem *load_system(FILE *sys)
{
    config_t config;
    config_init(&config);
    
    if (!config_read(&config, sys)) {
        fprintf(stderr, "Config parsing error: %s at line %d in file %s\n", config_error_text(&config),
                                                                            config_error_line(&config),
                                                                            config_error_file(&config));
        goto err;
    }
    
    config_setting_t *setting;
    
    setting = config_lookup(&config, "system.planets");
    
    if (setting == NULL) {
        fprintf(stderr, "Couldn't find planets in config file.\n");
        goto err;
    }
    
    uint64_t count = config_setting_length(setting);
    
    //Allocate space for the bodies
    Body *planets = calloc(count, sizeof(Body));
    
err:
    config_destroy(&config);
    return NULL;
}