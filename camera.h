#ifndef _CAMERA_H
#define _CAMERA_H

#include "math.h"
#include "ray.h"

typedef struct camera_t
{
    point_3d origin;
    vector_3d u, v, w;
    int x_resolution, y_resolution;
    fixed_point _width, _height;
} camera_t;

void init_camera(point_3d *origin, vector_3d *up, vector_3d *forward, int x_resolution, int y_resolution, fixed_point fov, camera_t *out);

void generate_ray(camera_t *camera, fixed_point x, fixed_point y, ray_t *out);

#endif