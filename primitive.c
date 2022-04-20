#include "primitive.h"

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

int _try_intersect_sphere(sphere_t *sphere, ray_t *ray, int max_distance, intersection_t *out);
int _try_intersect_rectangle(rectangle_t *rectangle, ray_t *ray, int max_distance, intersection_t *out);
void _shade_diffuse(colour *diffuse_colour, fixed_point kd, scene_t *scene, intersection_t *intersection, colour *out);

int try_intersect(primitive_t *primitive, ray_t *ray, int max_distance, intersection_t *out)
{
    if (primitive->type == SPHERE)
    {
        out->primitive = *primitive;
        return _try_intersect_sphere(&primitive->as.sphere, ray, max_distance, out);
    }
    if (primitive->type == RECTANGLE)
    {
        out->primitive = *primitive;
        return _try_intersect_rectangle(&primitive->as.rectangle, ray, max_distance, out);
    }
    printf("Unsupported type of primitive: %d\n", primitive->type);
    return 0;
}

int _try_intersect_sphere(sphere_t *sphere, ray_t *ray, int max_distance, intersection_t *out)
{
    vector_3d oc, ocd;
    fixed_point a, b, c, disc, root, div, min, max;

    vector_subtract(&ray->origin, &sphere->center, &oc);
    a = vector_dot(&ray->direction, &ray->direction);
    vector_scale(&oc, FP(2), &ocd);
    b = vector_dot(&ocd, &ray->direction);
    c = vector_dot(&oc, &oc) - FP_MUL(sphere->radius, sphere->radius);
    disc = fp_mul_ext(b, b) - 4 * FP_MUL(a, c);

    if (disc < 0)
    {
        return 0;
    }
    root = csqrt(disc);
    div = 2 * a;

    min = FP_DIV(-b - root, div);
    if (min >= 0 && min < max_distance)
    {
        // use oc as temp for calculating hitpoint
        vector_scale(&ray->direction, min, &oc);
        vector_add(&ray->origin, &oc, &out->hitpoint);
        // calculate normal
        vector_subtract(&out->hitpoint, &sphere->center, &out->normal);
        VectorNormal(&out->normal, &out->normal);
        out->distance = min;
        return 1;
    }

    max = FP_DIV(-b + root, div);
    if (max >= 0 && max < max_distance)
    {
        // use oc as temp for calculating hitpoint
        vector_scale(&ray->direction, max, &oc);
        vector_add(&ray->origin, &oc, &out->hitpoint);
        // calculate normal
        vector_subtract(&out->hitpoint, &sphere->center, &out->normal);
        VectorNormal(&out->normal, &out->normal);
        out->distance = max;
        return 1;
    }
    return 0;
}

int _try_intersect_rectangle(rectangle_t *rectangle, ray_t *ray, int max_distance, intersection_t *out)
{
    vector_3d rc, segment, diagonal, squared;
    point_3d hitpoint;
    fixed_point distance, dot, length;

    point_subtract(&rectangle->corner, &ray->origin, &rc);
    distance = FP_DIV(vector_dot(&rc, &rectangle->normal), vector_dot(&ray->direction, &rectangle->normal));

    if (distance < 0)
    {
        return 0;
    }
    vector_scale(&ray->direction, distance, &segment);
    point_translate(&ray->origin, &segment, &hitpoint);
    point_subtract(&hitpoint, &rectangle->corner, &diagonal);

    dot = vector_dot(&diagonal, &rectangle->side1);
    Square12(&rectangle->side1, &squared);
    if (dot < 0 || dot > squared.vx + squared.vy + squared.vz)
    {
        return 0;
    }

    dot = vector_dot(&diagonal, &rectangle->side2);
    Square12(&rectangle->side2, &squared);
    if (dot < 0 || dot > squared.vx + squared.vy + squared.vz)
    {
        return 0;
    }
    return 1;
}

void shade(primitive_t *primitive, scene_t *scene, intersection_t *intersection, colour *out)
{
    if (intersection->primitive.type == SPHERE)
    {
        _shade_diffuse(&intersection->primitive.as.sphere.colour, intersection->primitive.as.sphere.kd, scene, intersection, out);
        return;
    }
    if (intersection->primitive.type == RECTANGLE)
    {
        _shade_diffuse(&intersection->primitive.as.rectangle.colour, intersection->primitive.as.rectangle.kd, scene, intersection, out);
        return;
    }
    printf("Unsupported type of primitive: %d\n", intersection->primitive.type);
}

void _shade_diffuse(colour *diffuse_colour, fixed_point kd, scene_t *scene, intersection_t *intersection, colour *out)
{
    vector_3d light_ray;
    colour colour;
    fixed_point angle, tmp, ip;
    point_light_t *light;

    light = &scene->lights[0].as.point_light;

    vector_subtract(&light->location, &intersection->hitpoint, &light_ray);
    VectorNormal(&light_ray, &light_ray);
    angle = vector_dot(&intersection->normal, &light_ray);

    if (angle <= 0)
    {
        setVector(out, 0, 0, 0);
        return;
    }
    // TODO: check for occlusion
    vector_scale(&light->colour, light->strength, &colour);

    tmp = FP_MUL(angle, kd);
    tmp = FP_MUL(tmp, FP_INV_PI);
    vector_scale(&colour, tmp, &colour);
    vector_multiply(diffuse_colour, &colour, out);
}