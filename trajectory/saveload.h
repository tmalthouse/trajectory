//
//  saveload.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/18/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef saveload_h
#define saveload_h

#include <stdio.h>
#include "body.h"


SolarSystem load_and_calculate_system(const char *filename);
int store_system (SolarSystem s, const char *db_filename);


#endif /* saveload_h */
