#include "raytracer.h"

#include "colour.h"
#include "intersection.h"
#include "ray.h"

void _ray_to_world_colour(ray_t *ray, colour *bg_floor, colour *bg_ceiling, colour *out);

void render_pixel(scene_t *scene, int x, int y, CVECTOR *out)
{
    ray_t ray;
    intersection_t intersection;
    colour colour;
    int i;
    int min_distance = FP(100000);

    generate_ray(&scene->camera, FP(x) + FP_HALF, FP(scene->camera.y_resolution - y - 1) + FP_HALF, &ray);
    // generate_ray(&scene->camera, FP(x) + FP_HALF, FP(y) + FP_HALF, &ray);

    for (i = 0; i < scene->primitive_count; i++)
    {
        if (try_intersect(&scene->primitives[i], &ray, min_distance, &intersection))
        {
            shade(&scene->primitives[i], scene, &intersection, &colour);
            out->r = colour.vx * 255 / FP_SCALE;
            out->g = colour.vy * 255 / FP_SCALE;
            out->b = colour.vz * 255 / FP_SCALE;
            gamma_correct(out, out);
            return;
        }
    }

    _ray_to_world_colour(&ray, &scene->bg_floor, &scene->bg_ceiling, &colour);
    out->r = colour.vx * 255 / FP_SCALE;
    out->g = colour.vy * 255 / FP_SCALE;
    out->b = colour.vz * 255 / FP_SCALE;
}

void _ray_to_world_colour(ray_t *ray, colour *bg_floor, colour *bg_ceiling, colour *out)
{
    fixed_point fraction;
    vector_3d direction;
    colour floor, ceiling;

    vector_normalize(&ray->direction, &direction);
    fraction = (direction.vy + FP_ONE) / 2;

    vector_scale(bg_floor, FP_ONE - fraction, &floor);
    vector_scale(bg_ceiling, fraction, &ceiling);

    vector_add(&floor, &ceiling, out);
}