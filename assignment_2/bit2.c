#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "mem.h"
#include "bit2.h"

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
	
	// total number of characters stored in array
	int words = ((height * width)/4) + 1;
	
	// allocate space for bit2 array and return struct
	bit2->array = CALLOC(words, sizeof(char));
	return bit2;
}

// function to free allocated memory on the heap
void Bit2_free(T *bit2){
	// assert valid Bit2_t
	assert(bit2 && *bit2);
	// free allocated space for array struct member
	FREE((*bit2)->array);
	// free allocated space for struct member
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
int Bit2_put(T bit2, int height, int width, int bit){
	// assert valid array, location and bit value
	assert(bit2);
	assert(height>=0 && height<(bit2->height));
	assert(width>=0 && width<(bit2->width));
	assert(bit==0 || bit==1);
	
	// find char in array
	int n = (height * bit2->width) + width;
	int prev;
	prev = ((bit2->array[n/4]>>(n%4))&1);

	if(bit==1)
		bit2->array[n/4] |=   1<<(n%4);
	else
		bit2->array[n/4] &= ~(1<<(n%4));
	return prev;
}

int Bit2_get(T bit2, int height, int width){
	assert(bit2);
	assert(height>=0 && height<(bit2->height));
	assert(width>=0 && width<(bit2->width));

	int n = (height * bit2->width) + width;

	return ((bit2->array[n/4]>>(n%4))&1);
}

void Bit2_map_row_major(T bit2, void apply(int n, int bit, void *cl), void *cl){
	for(int i=0; i<((bit2->height)*(bit2->width)); i++){
		apply(i, ((bit2->array[i/4]>>(i%4))&1), cl);
	}
	return;
}

void Bit2_map_col_major(T bit2, void apply(int n, int bit, void *cl), void *cl){
	for(int i=0; i<(bit2->width); i++){
		for(int j=0; j<(bit2->height); j++){
			apply(i, ((bit2->array[i/4]>>(i%4))&1), cl);
		}
	}
	return;
}

