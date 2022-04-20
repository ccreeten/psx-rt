#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "colour.h"
#include "light.h"
#include "math.h"
#include "ray.h"

typedef enum primitive_type
{
    SPHERE,
    RECTANGLE
} primitive_type;

typedef struct sphere_t
{
    point_3d center;
    fixed_point radius;
    colour colour;
    fixed_point kd;
} sphere_t;

typedef struct rectangle_t
{
    point_3d corner;
    vector_3d side1, side2, normal;
    colour colour;
    fixed_point kd;
} rectangle_t;

typedef struct 
{
    primitive_type type;
    union
    {
        sphere_t sphere;
        rectangle_t rectangle;
    } as;
} primitive_t;

#include "intersection.h"
#include "scene.h"

int try_intersect(primitive_t *primitive, ray_t *ray, int max_distance, intersection_t *out);
void shade(primitive_t *primitive, scene_t *scene, intersection_t *intersection, colour *out);

#endif