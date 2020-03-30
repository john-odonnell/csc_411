#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "pnm.h"

#ifndef FLOAT_CONVERT_INCLUDED
#define FLOAT_CONVERT_INCLUDED

typedef struct Pnm_rgb_float {
	float red, green, blue;
} *Pnm_rgb_float;

extern A2Methods_Array2 *to_float(Pnm_ppm *image, int width, int height, int denom);

#endif
