//
//  color.c
//  trajectory
//
//  Created by Thomas Malthouse on 7/27/16.
//  Copyright © 2016 Thomas Malthouse. All rights reserved.
//

#include "color.h"
#include "debug.h"

//Checks the endianness of the system. Returns 0 if big, 1 if little.
int check_endianness () {
    volatile uint32_t i = 0x000000FF;//Don't compile this away
    return (*((uint8_t*)(&i)))==0xFF;
}

//For both these functions, we treat the hex value (a 32-bit uint) like an array of 4 8-bit values--a, b, g, r in that order.
Color hex_to_color(uint32_t hex)
{
    //We're assuming intel procs here--little endian.
    Color result;
    uint8_t *hex_ptr = (uint8_t*)&hex;
    dblogger("%x\n", hex);
    if (check_endianness()) {//If it's little endian (ie intel)
        result.a = hex_ptr[3];
        result.r = hex_ptr[2];
        result.g = hex_ptr[1];
        result.b = hex_ptr[0];
    } else {
        result.a = hex_ptr[0];
        result.r = hex_ptr[1];
        result.g = hex_ptr[2];
        result.b = hex_ptr[3];
    }
    return result;
}

uint32_t color_to_hex(Color c)
{
    uint32_t hex = 0;
    uint8_t *hex_ptr = (uint8_t*)&hex;
    if (check_endianness()) {//If it's little endian (ie intel)
        hex_ptr[0] = c.b;
        hex_ptr[1] = c.g;
        hex_ptr[2] = c.r;
        hex_ptr[3] = c.a;
    } else {
        hex_ptr[3] = c.b;
        hex_ptr[2] = c.g;
        hex_ptr[1] = c.r;
        hex_ptr[0] = c.a;
    }
    return hex;
}