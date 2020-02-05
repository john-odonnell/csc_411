static const char *rcsid = "$Id$";
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#define T UArray2_T

struct T{
	int height;
	int width;
	int size;
	char **array;
};

void UArray2_init(T uarray2, int height, int width, int size, char **ary){
	assert(uarray2);
	assert(ary && height>0 || height==0 && ary==NULL);
	assert(size>0);

	uarray2->height = height;
	uarray2->width = width;
	uarray2->size = size;

	if(height>0)
		uarray2->array = ary;
	else
		uarray2->array = NULL;

	for(int i=0;i<height;i++){
		Array_T array;
		array = Array_new(width, size);
		*(uarray2->ary + sizeof(char *)*i) = &array;
	}
}

void UArray2_new(int height, int width, int size){
	T uarray2;
	NEW(uarray2);
	if(height>0 && width>0)
		UArray2_init(uarray2, height, width, size, CALLOC(height, sizeof(char *)));
	else
		UArray2_init(uarray2, height, width, size, NULL);
	return uarray2
}

void UArray2_free(T *uarray2){
	assert(uarray2 && *uarray2);
	for(int i=0;i<height;i++){
		Array_free(*((*uarray2)->array + i*uarray2->sizeof(char *)));
	}
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
	
	return Array_get(**(uarray2->array + sizeof(char *)*height), width);
}



