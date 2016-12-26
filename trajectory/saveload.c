//
//  saveload.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/18/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "saveload.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include "body.h"
#include "debug.h"

#include "SQL_statements.h"

/// Types used by the body-construction mechanism
struct db_body {
  Body b;
  uint64_t db_id;
  uint64_t root_id;
};

/*~/~/~/~/~/~/~/~/~/~*/

static uint64_t djb2_hash(const char *str) {
  uint64_t hash = 5381;
  int c;

  while ((c = *str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

int count_callback(void *dest, int argc, char **argv, char **col_names) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(col_names[i], "count")) {
      *((uint64_t *)dest) = strtoull(argv[i], NULL, 0);
      return 0;
    }
  }
  // Return an error if none of the columns match "count", which typically
  // indicates a misconfigured SQL statement.
  logger("Couldn't get number of rows in the table 'bodies'. Does it exist?");
  return 1;
}

static int fill_body_field(sqlite3_stmt *stmt, struct db_body *b, int icol) {
  uint64_t col_name_hashed = djb2_hash(sqlite3_column_name(stmt, icol));

  if (djb2_hash("id") == col_name_hashed) {
    int id = sqlite3_column_int(stmt, icol);
    b->db_id = id;
    b->b.id = id;
  } else if (djb2_hash("root_id") == col_name_hashed) {
    b->root_id = sqlite3_column_int(stmt, icol);
  } else if (djb2_hash("color") == col_name_hashed) {
    b->b.color = hex_to_color((uint32_t)strtol(
        (const char *)sqlite3_column_text(stmt, icol), NULL, 16));
  } else if (djb2_hash("name") == col_name_hashed) {
    strlcpy(b->b.name, (const char *)sqlite3_column_text(stmt, icol),
            BODY_NAME_MAX_LEN);
  } else if (djb2_hash("mass") == col_name_hashed) {
    b->b.mass = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("semimajoraxis") == col_name_hashed) {
    b->b.orbit.sma = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("eccentricity") == col_name_hashed) {
    b->b.orbit.ecc = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("inclination") == col_name_hashed) {
    b->b.orbit.inc = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("arg_of_periapsis") == col_name_hashed) {
    b->b.orbit.ape = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("long_ascend_node") == col_name_hashed) {
    b->b.orbit.lan = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("mean_ano_epoch") == col_name_hashed) {
    b->b.orbit.mna = sqlite3_column_double(stmt, icol);
  } else if (djb2_hash("time_since_epoch") == col_name_hashed) {
    b->b.orbit.epoch = sqlite3_column_int64(stmt, icol);
  }

  return 0;
}

static SolarSystem load_system(const char *filename) {
  SolarSystem system = {};

  sqlite3 *db;
  int status = sqlite3_open(filename, &db);
  if (status) {
    goto error;
  }

  char *err = NULL;
  int rows;
  status = sqlite3_exec(db, COUNTBODIES, count_callback, &rows, &err);
  if (status) {
    goto error;
  }

  system.planets = calloc(sizeof(Body), rows);
  system.count = rows;

  struct db_body *body_buffer = calloc(sizeof(struct db_body), rows);

  sqlite3_stmt *getbodies;
  status =
      sqlite3_prepare_v2(db, GETBODIES, NULL_TERMINATED_STMT, &getbodies, NULL);
  if (status) {
    goto error;
  }

  for (int i = 0; i < rows; i++) {
    status = sqlite3_step(getbodies);
    if (status != SQLITE_ROW) {
      goto error;
    }

    int col_count = sqlite3_column_count(getbodies);
    for (int j = 0; j < col_count; j++) {
      fill_body_field(getbodies, &(body_buffer[i]), j);
    }
  }

  sqlite3_finalize(getbodies);

  // Now, we need to copy the buffer into the returned solar system
  for (uint64_t i = 0; i < system.count; i++) {
    system.planets[i] = body_buffer[i].b;
  }

  // And match up the root body pointers)
  for (uint64_t i = 0; i < system.count; i++) {
    uint64_t root_id = body_buffer[i].root_id;

    // If the body doesn't have a root id--i.e. it is the root.
    if (root_id == 0) {
      system.planets[i].orbit.parent = NULL;
    }

    // Now we find the corresponding body
    for (uint64_t j = 0; j < system.count; j++) {
      if (root_id == body_buffer[j].db_id) {
        system.planets[i].orbit.parent = &(system.planets[j]);
        break;
      }
    }
  }

  for (uint64_t i = 0; i < system.count; i++) {
    double mass = system.planets[i].mass;
    double radius = pow(mass / M_SUN, 0.1) * 5;
    if (radius < 1) radius = 1;

    system.planets[i].screensize = (uint16_t)rint(radius);
  }

  free(body_buffer);
  logger("Finished loading solar system!");
  sqlite3_close(db);
  return system;

error:
  logger("Database error: %s\n", sqlite3_errmsg(db));
  sqlite3_close(db);
  exit(EXIT_FAILURE);
}

static void system_calculate_state_vectors(SolarSystem *s) {
  for (uint64_t i = 0; i < s->count; i++) {
    calculate_state_vectors(&s->planets[i], s->t);
  }
  return;
}

SolarSystem load_and_calculate_system(const char *filename) {
  SolarSystem s = load_system(filename);

  for (uint64_t i = 0; i < s.count; i++) {
    s.planets[i].mu = calculate_mu(s.planets[i]);
  }

  system_calculate_state_vectors(&s);
  for (uint64_t i = 0; i < s.count; i++) {
    print_body_info(s.planets[i]);
  }
  return s;
}

// And here we have functions for writing to the database
/*~*~*~*~*~*~*~*~*~*/

int store_system(SolarSystem s, const char *db_filename) {
  logger("Saving state...");

  sqlite3 *db;
  int status = sqlite3_open(db_filename, &db);
  if (status) {
    goto error;
  }

  sqlite3_stmt *stmt;
  status =
      sqlite3_prepare_v2(db, UPDATEBODY, NULL_TERMINATED_STMT, &stmt, NULL);
  if (status) {
    goto error;
  }

  /*"UPDATE bodies SET semimajoraxis='?1', eccentricity='?2', inclination='?3',
   * arg_of_periapsis=':?4', long_ascend_node='?5', mean_ano_epoch='?6',
   * time_since_epoch='?7' WHERE id='?8'"*/
  for (uint64_t i = 0; i < s.count; i++) {
    Body b = s.planets[i];
    logger("Saving %s...", b.name);

    if (b.orbit.sma == 0) {
      // We don't need to update the root body's orbit, since it's root.
      logger("Done saving body %s as root", b.name);
      continue;
    }
    int bind_status = 0;

    bind_status += sqlite3_bind_double(stmt, 1, b.orbit.sma);
    bind_status += sqlite3_bind_double(stmt, 2, b.orbit.ecc);
    bind_status += sqlite3_bind_double(stmt, 3, b.orbit.inc);
    bind_status += sqlite3_bind_double(stmt, 4, b.orbit.ape);
    bind_status += sqlite3_bind_double(stmt, 5, b.orbit.lan);
    bind_status += sqlite3_bind_double(stmt, 6, b.orbit.mna);
    bind_status += sqlite3_bind_double(stmt, 7, b.orbit.epoch);

    bind_status += sqlite3_bind_int(stmt, 8, (int)b.id);

    if (bind_status) {
      goto error;
    }

    status = 0;
    status += (sqlite3_step(stmt) == SQLITE_DONE);

    status += sqlite3_clear_bindings(stmt);
    status += (sqlite3_reset(stmt) == SQLITE_OK);

    if (status) {
      goto error;
    }

    logger("Done saving body %s", b.name);
  }

  sqlite3_finalize(stmt);

  return 0;

error:

  logger("Database error: %s\n", sqlite3_errmsg(db));
  sqlite3_close(db);
  return 1;
}
