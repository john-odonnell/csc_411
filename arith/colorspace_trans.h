#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"

#ifndef COLORSPACE_TRANS_INCLUDED
#define COLORSPACE_TRANS_INCLUDED

typedef struct Pnm_colorspace {
	float Y, Pb, Pr;
} *Pnm_colorspace;

extern void to_colorspace(A2Methods_Array2 *arr, int width, int height);

#endif
