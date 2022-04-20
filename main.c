#include <stdio.h>
#include <sys/types.h>
#include <libapi.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <malloc.h>

#include "camera.h"
#include "light.h"
#include "math.h"
#include "primitive.h"
#include "raytracer.h"
#include "scene.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define RCNT1_TICKS_S 15733
#define RCNT1_INT 0x1000

typedef struct gfx_env_t
{
    DISPENV disp_env;
    DRAWENV draw_env;
} gfx_env_t;

int init_font(void);
void init_gfx(void);
void display(int stream_id);

void init_scene(scene_t *scene);
void setup_camera(scene_t *scene);
void setup_primitives(scene_t *scene);
void setup_lights(scene_t *scene);
void setup_background(scene_t *scene);

u_long heap_head = 0x80100000;
u_long heap_size = 0x80000;

u_long ot_head;
pixel buffer[SCREEN_WIDTH];

int main()
{
    TILE fnt_back;

    int stream_id;

    int x;
    int y;
    int done;
    pixel *tile;

    scene_t scene;

    CVECTOR colour;

    int start;
    int time;

    int h_ticks;

    InitHeap3(&heap_head, heap_size);
    InitGeom();
    init_gfx();
    stream_id = init_font();
    init_scene(&scene);

    x = 0;
    y = 0;
    done = 0;

    SetRCnt(RCntCNT1, 65535, RCNT1_INT);
    StartRCnt(RCntCNT1);

    while (1)
    {
        if (!done)
        {
            ResetRCnt(RCntCNT1);
            start = GetRCnt(RCntCNT1);
            // ClearOTagR(&head, 1);
            ClearOTag(&ot_head, 1);

            for (x = 0; x < SCREEN_WIDTH; x++)
            {
                tile = &buffer[x];
                setTile1(tile);
                setXY0(tile, x, y);
                render_pixel(&scene, x, y, &colour);
                setRGB0(tile, colour.r, colour.g, colour.b);
                addPrim(&ot_head, tile);
            }
            y++;
            done |= y >= SCREEN_HEIGHT;

            if (done || (y % (64 + 8)) == 0)
            {
                setTile(&fnt_back);
                setXY0(&fnt_back, 8, 8);
                setWH(&fnt_back, 256, 64);
                setRGB0(&fnt_back, 0, 0, 0);
                // does not work obviously, because I do not clear
                // setSemiTrans(&fnt_back, 1);
                addPrim(&ot_head, &fnt_back);
            }

            h_ticks += GetRCnt(RCntCNT1) - start;

            if (done || (y % (64 + 8)) == 0)
            {
                FntPrint(stream_id, "render stats:\n");
                FntPrint(stream_id, "\n");
                FntPrint(stream_id, "\t%6d px in %ds\n", y * SCREEN_WIDTH, h_ticks / RCNT1_TICKS_S);
                FntPrint(stream_id, "\n");
                FntPrint(stream_id, "\t%6d px/s\n", y * SCREEN_WIDTH / (h_ticks / RCNT1_TICKS_S));
            }

            display(stream_id);
        }
    }

    free3(scene.lights);
    free3(scene.primitives);

    return 0;
}

int init_font(void)
{
    FntLoad(960, 0);
    return FntOpen(16, 16, SCREEN_WIDTH - (16 * 2), SCREEN_HEIGHT - (16 * 2), 0, 128);
}

void init_gfx()
{
    gfx_env_t gfx_env;
    ResetGraph(0);
    SetDefDispEnv(&gfx_env.disp_env, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&gfx_env.draw_env, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    PutDispEnv(&gfx_env.disp_env);
    PutDrawEnv(&gfx_env.draw_env);

    SetDispMask(1);
}

void display(int stream_id)
{
    // DrawSync(0);
    while (DrawSync(1));
    VSync(0);
    DrawOTag(&ot_head);
    FntFlush(stream_id);
}

void init_scene(scene_t *scene)
{
    setup_camera(scene);
    setup_primitives(scene);
    setup_lights(scene);
    setup_background(scene);
}

void setup_camera(scene_t *scene)
{
    point_3d origin;
    vector_3d up, direction;

    setVector(&origin, FP(+0), FP(+8), FP(+8));
    setVector(&up, FP(+0), FP(+1), FP(+0));
    setVector(&direction, FP(+0), FP(-1), FP(-1));

    init_camera(&origin, &up, &direction, SCREEN_WIDTH, SCREEN_HEIGHT, FP_DEG(45), &scene->camera);
}

void setup_primitives(scene_t *scene)
{
    sphere_t sphere;
    rectangle_t rectangle;
    int count = 3;

    scene->primitive_count = count;
    scene->primitives = (primitive_t *)malloc3(sizeof(primitive_t) * scene->primitive_count);

    scene->primitives[0].type = SPHERE;
    setVector(&sphere.center, FP(-25) / 10, FP_HALF, 0);
    sphere.radius = FP_ONE;
    setVector(&sphere.colour, 0, FP_ONE, FP_ONE);
    sphere.kd = FP(64) / 100;
    scene->primitives[0].as.sphere = sphere;

    scene->primitives[1].type = SPHERE;
    setVector(&sphere.center, 0, FP_HALF, 0);
    sphere.radius = FP_ONE;
    setVector(&sphere.colour, FP_ONE, 0, FP_ONE);
    sphere.kd = FP(64) / 100;
    scene->primitives[1].as.sphere = sphere;

    scene->primitives[2].type = SPHERE;
    setVector(&sphere.center, FP(+25) / 10, FP_HALF, 0);
    sphere.radius = FP_ONE;
    setVector(&sphere.colour, FP_ONE, FP_ONE, 0);
    sphere.kd = FP(64) / 100;
    scene->primitives[2].as.sphere = sphere;

    // scene->primitives[--count].type = RECTANGLE;
    // setVector(&rectangle.corner, FP(-3), 0, FP(-3));
    // setVector(&rectangle.side1, FP(6), 0, 0);
    // setVector(&rectangle.side2, 0, 0, FP(6));
    // setVector(&rectangle.normal, 0, FP_ONE, 0);
    // setVector(&rectangle.colour, FP(172) / 256, FP(172) / 256, FP(172) / 256);
    // rectangle.kd = FP(64) / 100;
    // scene->primitives[count].as.rectangle = rectangle;
}

void setup_lights(scene_t *scene)
{
    point_light_t point_light;

    scene->light_count = 1;
    scene->lights = (light_t *)malloc3(sizeof(light_t) * scene->light_count);

    scene->lights[0].type = POINT;
    setVector(&point_light.location, FP(2), FP(4), 0);
    setVector(&point_light.colour, FP_ONE, FP_ONE, FP_ONE);
    point_light.strength = FP(3);
    scene->lights[0].as.point_light = point_light;

    point_light = scene->lights[0].as.point_light;
}

void setup_background(scene_t *scene)
{
    setVector(&scene->bg_floor, FP_ONE * 127 / 255, FP_ONE * 191 / 255, FP_ONE);
    setVector(&scene->bg_ceiling, FP_ONE, FP_ONE, FP_ONE);
}