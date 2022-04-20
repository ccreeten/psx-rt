#ifndef _SCENE_H
#define _SCENE_H

typedef struct scene_t scene_t;

#include "camera.h"
#include "colour.h"
#include "light.h"
#include "primitive.h"

struct scene_t
{
    camera_t camera;
    colour bg_floor, bg_ceiling;
    light_t *lights;
    int light_count;
    primitive_t *primitives;
    int primitive_count;
};

#endif