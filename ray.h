#ifndef _RAY_H
#define _RAY_H

#include "math.h"

typedef struct ray_t
{
    point_3d origin;
    vector_3d direction;
} ray_t;

#endif