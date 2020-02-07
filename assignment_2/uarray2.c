#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#include "uarray2.h"
#define T UArray2_T

T UArray2_new(int height, int width, int size){
	assert(height>0 && width >0 && size>0);

	T uarray2;
	NEW(uarray2);

	uarray2->height = height;
	uarray2->width = width;
	uarray2->size = size;

	uarray2->array = CALLOC(height*width, size);

	return uarray2;
}

void UArray2_free(T *uarray2){
	assert(uarray2 && *uarray2);
	FREE((*uarray2)->array);
	FREE(*uarray2);
}

int UArray2_height(T uarray2){
	assert(uarray2);
	return uarray2->height;
}

int UArray2_width(T uarray2){
	assert(uarray2);
	return uarray2->width;
}

int UArray2_size(T uarray2){
	assert(uarray2);
	return uarray2->size;
}

void *UArray2_at(T uarray2, int height, int width){
	assert(uarray2);
	assert(height>=0 && height<uarray2->height);
	assert(width>=0 && width<uarray2->width);
	
	return ((uarray2->array + (uarray2->size * height)) + (uarray2->size * width));
}

void UArray2_map_row_major(T uarray2, void apply(void *p, int bit, void *cl), void *cl){
 	for(int i=0; i < (uarray2->height * uarray2->width); i++){
 		int *n;
		NEW(n);
		n = (uarray2->array + (i * uarray2->size));
 		apply(n, i, cl);
	}
}

// void UArray_map_col_major(T uarray2, void apply(int n, int bit, void *cl), void *cl){
// 	for(int i = 0; i < uarray2->width; i++){
// 		for(int j = 0; j < uarray2->height; j++){
// 			char *p = ((uarray2->array + (uarray2->size * i)) + (uarray2->size * j));
// 			apply(p, *p, cl);
// 		}
// 	}
// }

