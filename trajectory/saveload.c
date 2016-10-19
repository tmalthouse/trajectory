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
#include <sqlite3.h>
#include "body.h"
#include "debug.h"

#include "SQL_statements.h"

struct db_body {
    Body b;
    uint64_t db_id;
    uint64_t root_id;
};

static int load_system__callback(void *NotUsed, int argc, char **argv, char **ColName){
//We define some macros to make code below simpler:
//I'm not entirely happy with function-internal macros, but it makes it easier to change (DRY vs macro hell i guess)
//We want to define DBL_VAL and LL_VAL as macros so that they are only executed when appropriate.
#define COL_IS(STR) (!strcmp(col_name, STR))
#define DBL_VAL (strtod(val, NULL))
#define LL_VAL (strtoll(val, NULL, 0))
    
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    struct db_body body = {0};
    
    for (uint64_t i=0; i<argc; i++) {
        char *val = argv[i];
        char *col_name = ColName[i];
        uint64_t body_id, root_id;
        
        //If-else ladder, assigning parts of Body based on the values in the table. God how I wish we could switch on strings
        if (COL_IS("name")) {
            strncpy(body.b.name, val, BODY_NAME_MAX_LEN);
            
        } else if(COL_IS("id")) {
            body_id = LL_VAL;
        } else if(COL_IS("root_id")) {
            root_id = LL_VAL;
        }
        
        //If the root id is the same as the body ID, the given body is root, and therefore has no orbital parameters
        if (body_id==root_id) {
            
        } else {
            if (COL_IS("semimajoraxis")) {
                body.b.orbit.sma = DBL_VAL;
                
            } else if (COL_IS("eccentricity")) {
                body.b.orbit.ecc = DBL_VAL;
                
            } else if (COL_IS("inclination")) {
                body.b.orbit.inc = DBL_VAL;
                
            } else if (COL_IS("arg_of_periapsis")) {
                body.b.orbit.ape = DBL_VAL;
                
            } else if (COL_IS("long_ascend_node")) {
                body.b.orbit.lan = DBL_VAL;
                
            } else if (COL_IS("mean_ano_epoch")) {
                body.b.orbit.mna = DBL_VAL;
                
            } else if (COL_IS("time_since_epoch")) {
                body.b.orbit.epoch = strtotime(val, NULL);
                
            }
        }
    }
    return 0;
    
//And undefine those macros, to avoid causing problems
#undef COL_IS
#undef DBL_VAL
#undef LL_VAL
}


SolarSystem load_system(char *filename)
{
    sqlite3 *db;
    int status = sqlite3_open(filename, &db);
    if (status) {
        goto error;
    }
    
    char *err = 0;
    status = sqlite3_exec(db, GETBODIES, load_system__callback, NULL, &err);
    
    return (SolarSystem){};
    
error:
    logger("Database error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(EXIT_FAILURE);
}

void system_calculate_state_vectors(SolarSystem *s)
{
    for (int i=0; i<s->count; i++) {
        calculate_state_vectors(&s->planets[i], s->t);
    }
    return;
}
