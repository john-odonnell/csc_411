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

// insert bit into 2d bit array at (height, width)
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

int Bit2_get(T bit2, int height, int width){
	assert(bit2);
	assert(height>=0 && height<(bit2->height));
	assert(width>=0 && width<(bit2->width));

	int n = (height * bit2->width) + width;
	return Bit_get(*(bit2->bit_array), n);
}

void Bit2_map_row_major(T bit2, void apply(int n, int bit, void *cl), void *cl){
	Bit_map(*(bit2->bit_array), apply, cl);
	return;
}

// void Bit2_map_col_major(T bit2, void apply(int n, int bit, void *cl), void *cl){
// 	int n;
// 	Bit_T *bit_arr;
// 	NEW(bit_arr);       
// 	bit_arr = bit2->bit_array;
// 	for(int i=0; i<(bit2->width); i++){
// 		for(int j=0; j<(bit2->height); j++){
// 			n = (j * bit2->width) + i;
// 			apply(i, (((*bit_arr)->bytes[n/8]>>(n%8))&1), cl);
// 		}
// 	}
// 	Bit_free(bit_arr);
// 	free(bit_arr);
// 	return;
// }

