#include "math.h"

#include <libgpu.h>

void vector_normalize(vector_3d *v0, vector_3d *output)
{
    fixed_point x, y, z, length, divisor;

    x = v0->vx;
    y = v0->vy;
    z = v0->vz;

    // TODO: inv_sqrt?
    length = csqrt(FP_MUL(x, x) + FP_MUL(y, y) + FP_MUL(z, z));
    divisor = FP_DIV(FP_ONE, length);

    output->vx = FP_MUL(x, divisor);
    output->vy = FP_MUL(y, divisor);
    output->vz = FP_MUL(z, divisor);
}

void vector_scale(vector_3d *v0, fixed_point scale, vector_3d *out)
{
    setVector(out, FP_MUL(v0->vx, scale), FP_MUL(v0->vy, scale), FP_MUL(v0->vz, scale));
}

void vector_add(vector_3d *v0, vector_3d *v1, vector_3d *out)
{
    setVector(out, v0->vx + v1->vx, v0->vy + v1->vy, v0->vz + v1->vz);
}

void vector_multiply(vector_3d *v0, vector_3d *v1, vector_3d *out)
{
    setVector(out, FP_MUL(v0->vx, v1->vx), FP_MUL(v0->vy, v1->vy), FP_MUL(v0->vz, v1->vz));
}

void vector_subtract(vector_3d *v0, vector_3d *v1, vector_3d *out)
{
    setVector(out, v0->vx - v1->vx, v0->vy - v1->vy, v0->vz - v1->vz);
}

void vector_cross(vector_3d *v0, vector_3d *v1, vector_3d *out)
{
    out->vx = ((v0->vy * v1->vz) - (v0->vz * v1->vy)) / FP_SCALE;
    out->vy = ((v0->vz * v1->vx) - (v0->vx * v1->vz)) / FP_SCALE;
    out->vz = ((v0->vx * v1->vy) - (v0->vy * v1->vx)) / FP_SCALE;
}

fixed_point vector_dot(vector_3d *v0, vector_3d *v1)
{
    return FP_MUL(v0->vx, v1->vx) + FP_MUL(v0->vy, v1->vy) + FP_MUL(v0->vz, v1->vz);
}

void point_subtract(point_3d *p0, point_3d *p1, vector_3d *out)
{
    setVector(out, p0->vx - p1->vx, p0->vy - p1->vy, p0->vz - p1->vz);
}

void point_translate(point_3d *p0, vector_3d *v0, point_3d *out)
{
    setVector(out, p0->vx + v0->vx, p0->vy + v0->vy, p0->vz + v0->vz);
}