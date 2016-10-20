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

int count_callback(void *dest, int argc, char **argv, char **col_names)
{
    for (int i=0; i<argc; i++) {
        if (!strcmp(col_names[i], "count")) {
            *((uint64_t*)dest) = strtoull(argv[i], NULL, 0);
            return 0;
        }
    }
    //Return an error if none of the columns match "count", which typically indicates a misconfigured SQL statement.
    logger("Couldn't get number of rows in the table 'bodies'. Does it exist?");
    return 1;
}

SolarSystem load_system(char *filename)
{
    SolarSystem system = {};
    
    sqlite3 *db;
    int status = sqlite3_open(filename, &db);
    if (status) {
        goto error;
    }
    
    char *err = NULL;
    uint64_t rows;
    status = sqlite3_exec(db, COUNTBODIES, count_callback, &rows, &err);
    if (status) {
        goto error;
    }
    
    system.planets = calloc(sizeof(Body), rows);
    system.count = rows;
    
    return system;
    
error:
    logger("Database error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(EXIT_FAILURE);
}

void system_calculate_state_vectors(SolarSystem *s)
{
    for (uint64_t i=0; i<s->count; i++) {
        calculate_state_vectors(&s->planets[i], s->t);
    }
    return;
}
