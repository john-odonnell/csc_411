#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"

#ifndef TO_AVERAGES_INCLUDED
#define TO_AVERAGES_INCLUDED

typedef struct block_components {
	unsigned  a;
	signed    b,  c,  d;
	unsigned avgPb, avgPr;
} *block_components;

extern A2Methods_Array2 *get_averages(A2Methods_Array2 *arr, int width, int height);

#endif