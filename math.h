#ifndef _MATH_H
#define _MATH_H

#include <sys/types.h>
#include <libgte.h>

#define FP_ONE ONE
#define FP_SCALE ONE
#define FP_HALF (FP_SCALE / 2)
#define FP_INV_PI 1303

#define FP(x) (((x)*FP_SCALE))
#define FP_FRAC(x, y) ((FP_ONE * (x)) / (y))
// only when both fixed point (you have double scaling)
#define FP_MUL(x, y) (((x) * (y)) / FP_SCALE)
// only when both fixed point (you lose scale)
#define FP_DIV(x, y) (((x) * FP_SCALE) / (y))
// degrees to playstation fixed point format
#define FP_DEG(x) (FP(x) / 360)

typedef VECTOR vector_3d;
typedef VECTOR point_3d;
typedef int fixed_point;

extern fixed_point fp_mul_ext(fixed_point multiplier, fixed_point multiplicant);

void vector_normalize(vector_3d *v0, vector_3d *out);
void vector_scale(vector_3d *v0, fixed_point scale, vector_3d *out);
void vector_add(vector_3d *v0, vector_3d *v1, vector_3d *out);
void vector_multiply(vector_3d *v1, vector_3d *v0, vector_3d *out);
void vector_subtract(vector_3d *v0, vector_3d *v1, vector_3d *out);
void vector_cross(vector_3d *v0, vector_3d *v1, vector_3d *out);

fixed_point vector_dot(vector_3d *v0, vector_3d *v1);

void point_subtract(point_3d *p0, point_3d *p1, vector_3d *out);
void point_translate(point_3d *p0, vector_3d *v0, point_3d *out);

#endif