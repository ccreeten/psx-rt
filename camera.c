#include "camera.h"

void init_camera(point_3d *origin, vector_3d *up, vector_3d *direction, int x_resolution, int y_resolution, fixed_point fov, camera_t *camera)
{
    fixed_point adjacent;

    adjacent = FP_DIV(rsin(fov / 2), rcos(fov / 2));

    camera->origin = *origin;
    camera->_width = adjacent * 2;
    camera->_height = FP_DIV(camera->_width, FP(x_resolution) / y_resolution);
    camera->x_resolution = x_resolution;
    camera->y_resolution = y_resolution;

    // construct orthonormal basis
    vector_scale(direction, FP(-1), direction);
    vector_normalize(direction, &camera->w);
    vector_cross(up, &camera->w, &camera->u);
    vector_normalize(&camera->u, &camera->u);
    vector_cross(&camera->w, &camera->u, &camera->v);
}

void generate_ray(camera_t *camera, fixed_point x, fixed_point y, ray_t *ray)
{
    fixed_point u, v;
    vector_3d uu, vv;

    u = FP_MUL(camera->_width, x / camera->x_resolution - FP_HALF);
    v = FP_MUL(camera->_height, y / camera->y_resolution - FP_HALF);

    ray->origin = camera->origin;
    vector_scale(&camera->u, u, &uu);
    vector_scale(&camera->v, v, &vv);
    vector_add(&uu, &vv, &ray->direction);
    vector_subtract(&ray->direction, &camera->w, &ray->direction);
    vector_normalize(&ray->direction, &ray->direction);
}