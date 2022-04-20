#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

#include "scene.h"

typedef TILE_1 pixel;

void render_pixel(scene_t *scene, int x, int y, CVECTOR *out);

#endif