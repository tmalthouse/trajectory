//
//  color.h
//  trajectory
//
//  Created by Thomas Malthouse on 7/27/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#ifndef color_h
#define color_h

#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} Color;

// Suppress warnings about enum values being above MAXINT, since it's not being
// used as an enum (the bits are right, even if it's not a valid enum value.)
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic push

// An enum containing the primary colors (plus black, white, grey, and teal).
// We're not supposed to use values above MAXINT for enums, but since the enum
// is just being used as a typed constant, we're fine.
typedef enum {
  COLOR_WHITE = 0xFFFFFFFF,
  COLOR_BLACK = 0xFF000000,
  COLOR_GRAY = 0xFF777777,
  COLOR_RED = 0xFFC72923,
  COLOR_ORANGE = 0xFFC3781C,
  COLOR_YELLOW = 0xFFEBF209,
  COLOR_GREEN = 0xFF2F7013,
  COLOR_BLUE = 0xFF313b8F,
  COLOR_TEAL = 0xFF008080
} BasicColor;

// Put the warnings back on
#pragma GCC diagnostic pop

// Converts a 32-bit rgba hex code (eg 0xFFFFFFFF) to a Color object.
Color hex_to_color(uint32_t);

// Returns the 32-bit rgba hex code (eg 0xFFFFFFFF) for a given Color object
uint32_t color_to_hex(Color c);

#endif /* color_h */
