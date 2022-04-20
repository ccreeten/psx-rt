#ifndef _LIGHT_H
#define _LIGHT_H

#include "math.h"
#include "colour.h"

typedef enum light_type
{
    POINT
} light_type;

typedef struct point_light_t
{
    point_3d location;
    colour colour;
    fixed_point strength;
} point_light_t;

typedef struct light_t
{
    light_type type;
    union {
        point_light_t point_light;
    } as;
} light_t;

#endif