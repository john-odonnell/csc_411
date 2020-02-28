#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "uarray2b.h"
#include "uarray2.h"
#include "array.h"
#include "mem.h"
#include "assert.h"

#define T UArray2b_T

struct T {
	// members
	int height;
	int width;
	int size;
	int blocksize;
	// points to a UArray2_T struct
	// which holds a pointer to an Array_T stuct
	// which holds an array of pointers to several Array_T's
	UArray2_T blocks;
	/* in the world of ideas, element (r,c) within the 2d array
	 * is located in the block represented by the
	 * (floor(r / blocksize), floor(c / blocksize)) element of the UArray2_T
	 * at the ((r % blocksize), (c % blocksize)) index */
};

T UArray2b_new (int width, int height, int size, int blocksize) {
	// assert valid dimensions
	assert(height>0 && width>0 && size>0 && blocksize>0);
	
	// allocate for new blocked 2d array
	T array2b;
	NEW(array2b);
	
	// assign members
	array2b->width = width;
	array2b->height = height;
	array2b->size = size;
	array2b->blocksize = blocksize;
	
	// find number of blocks, width and height of blocks
	int blocks_w, blocks_h;
	blocks_w = (int)ceil((double)width/(double)blocksize);
	blocks_h = (int)ceil((double)height/(double)blocksize);

	array2b->blocks = UArray2_new(blocks_w, blocks_h, sizeof(void *));	
	
	// each location in the 2d array of blocks holds a pointer to a
	// 1d array that holds the values stored within the overall structure
	for (int i=0; i<blocks_h; i++) {
		for (int j=0; j<blocks_w; j++) {
			Array_T *block;
			NEW(block);
			*block = Array_new((blocksize * blocksize), size);

			Array_T **block_ptr = UArray2_at(array2b->blocks, i, j);
			*block_ptr = block;
		}
	}

	return array2b;
}

T UArray2b_new_64K_block (int width, int height, int size) {
	int blocksize = (int)floor(sqrt(64000 / size));
	return UArray2b_new(width, height, size, blocksize);
}

void apply_free (int i, int j, UArray2_T uarray2, void *elem, void *cl) {
	(void)i; (void)j; (void)uarray2; (void)cl;
	// free the array pointed to by the contents of the array
	Array_free(*(Array_T **)elem);
	FREE(*(Array_T **)elem);
	return;
}

void UArray2b_free (T *array2b) {
	// assert valid blocked 2d array
	assert(array2b && *array2b);
	
	// map over the 2d array of blocks to deallocate underlying 1d arrays
	UArray2_map_row_major((*array2b)->blocks, apply_free, NULL);
	
	// free substructure UArray2
	UArray2_free(&(*array2b)->blocks);
	// free 2d blocked array
	FREE(*array2b);
}

int UArray2b_width (T array2b) {
	assert(array2b);
	return array2b->width;
}

int UArray2b_height (T array2b) {
	assert(array2b);
	return array2b->height;
}

int UArray2b_size (T array2b) {
	assert(array2b);
	return array2b->size;
}

int UArray2b_blocksize (T array2b) {
	assert(array2b);
	return array2b->blocksize;
}

void *UArray2b_at (T array2b, int i, int j) {
	// assert that i and j are within bounds
	assert(i<(array2b->height) && j<(array2b->width));
	
	// store blocksize
	int blocksize = array2b->blocksize;
	
	// determine location of block
	int block_i, block_j;
	block_i = i / blocksize;
	block_j = j / blocksize;
	
	// determine location within block
	int inner_i, inner_j;
	inner_i = i % blocksize;
	inner_j = j % blocksize;
	
	// gets a pointer to the block array by retreiving the pointer stored at the location
	// (block_i, block_j)
	Array_T **block_ptr_loc = (Array_T **)UArray2_at(array2b->blocks, block_i, block_j);
	Array_T *block = *block_ptr_loc;
	// return the memory location of the element inthe block array
	return Array_get(*block, ((inner_i * blocksize) + inner_j));
}

void UArray2b_map (T array2b, void apply(int i, int j, T array2b, void *elem, void *cl), void *cl) {
	// get array details
	int blocksize = UArray2b_blocksize(array2b);
	int h_max = UArray2b_height(array2b);
	int w_max = UArray2b_width(array2b);
	int height_b = (int)(ceil((double)h_max / (double)blocksize));
	int width_b = (int)(ceil((double)w_max / (double)blocksize));
	
	// void pointer to elements
	void *p;
	
	// for each row of blocks
	for (int i=0; i<height_b; i++) {
		// for each column of blocks
		for (int j=0; j<width_b; j++) {
			// for each row within a block
			for (int k=(i * blocksize); k<((i * blocksize) + blocksize); k++) {
				// for each column within a block
				for (int l=(j * blocksize); l<((j * blocksize) + blocksize); l++) {
					// if the element contains valid data
					if (k < h_max && l < w_max) {
						// set pointer
						p = UArray2b_at(array2b, k, l);
						// call apply function
						apply(k, l, array2b, p, cl);
					}
				}
			}
		}
	}
}
