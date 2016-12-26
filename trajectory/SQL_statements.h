//
//  SQL_statements.h
//  trajectory
//
//  Created by Thomas Malthouse on 10/18/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef SQL_statements_h
#define SQL_statements_h

#define NULL_TERMINATED_STMT (-1)

static const char *COUNTBODIES = "SELECT COUNT(*) AS count FROM bodies";
static const char *GETBODIES = "SELECT * FROM bodies";
static const char *UPDATEBODY =
    "UPDATE bodies SET semimajoraxis=?001, eccentricity=?002, "
    "inclination=?003, arg_of_periapsis=?004, long_ascend_node=?005, "
    "mean_ano_epoch=?006, time_since_epoch=?007 WHERE id=?008";

#endif /* SQL_statements_h */
