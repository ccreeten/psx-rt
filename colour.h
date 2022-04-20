#ifndef _COLOUR_H
#define _COLOUR_H

#include <sys/types.h>
#include <libgte.h>

typedef VECTOR colour;

void gamma_correct(CVECTOR *c0, CVECTOR *out);

#endif