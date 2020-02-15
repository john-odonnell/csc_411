#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "mem.h"
#include "bit2.h"
#include "bit.h"

#define T Bit2_T

// initializes new 2d bit array
T Bit2_new(int height, int width){
	// assert valid height and width
	assert(height>0 && width>0);
	
	// create new Bit2_T and allocatie space
	T bit2;
	NEW(bit2);
	
	// set struct members
	bit2->height = height;
	bit2->width = width;
	
	Bit_T *bit_arr;
	NEW(bit_arr);
	*bit_arr = Bit_new(height*width);
	
	bit2->bit_array = bit_arr;
	return bit2;
}

// function to free allocated memory on the heap
void Bit2_free(T *bit2){
	// assert valid Bit2_t
	assert(bit2 && *bit2);
	Bit_free((*bit2)->bit_array);
	FREE((*bit2)->bit_array);
	FREE(*bit2);
}

// return height of 2d bit array
int Bit2_height(T bit2){
	assert(bit2);
	return bit2->height;
}

// return width of 2d bit array
int Bit2_width(T bit2){
	assert(bit2);
	return bit2->width;
}

// insert bit into 2d bit array at (i, j)
// (i, j) translates to the one dimensional idx n by the realtionship
// n = (i * bit2->width) + j
int Bit2_put(T bit2, int i, int j, int bit){
	// assert valid array, location and bit value
	assert(bit2);
	assert(i>=0 && i<(bit2->height));
	assert(j>=0 && j<(bit2->width));
	assert(bit==0 || bit==1);
	
	// find char in array
	int n = (i * bit2->width) + j;
	return Bit_put(*(bit2->bit_array), n, bit);
}

// returns the bit stored in the 2d bit array at (i, j)
// uses the same relationship as Bit2_put (see above)
int Bit2_get(T bit2, int i, int j){
	assert(bit2);
	assert(i>=0 && i<(bit2->height));
	assert(j>=0 && j<(bit2->width));

	int n = (i * bit2->width) + j;
	return Bit_get(*(bit2->bit_array), n);
}

// maps the apply function onto each bit in the 2d bit array
// maps in row major order (j increments faster than i)
void Bit2_map_row_major(T bit2, void apply(int n, int bit, void *cl), void *cl){
	Bit_map(*(bit2->bit_array), apply, cl);
	return;
}
