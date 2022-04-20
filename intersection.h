#ifndef _INTERSECTION_H
#define _INTERSECTION_H

typedef struct intersection_t intersection_t;

#include "math.h"
#include "primitive.h"

struct intersection_t
{
    point_3d hitpoint;
    vector_3d normal;
    fixed_point distance;
    primitive_t primitive;
};

#endif