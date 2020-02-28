#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"

void apply_print (int i, int j, A2Methods_Array2 array2b, void *elem, void *cl) {
	(void)array2b; (void)cl;
	printf("i: %d\tj: %d\tloc: %p\tval: %d\n", i, j, elem, *(int *)elem);
}

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
	A2Methods_T methods = array2_methods_blocked;
	A2Methods_Array2 array2b = methods->new_with_blocksize(10, 10, sizeof(int), 3);

	// UArray2b_T *array2b;
	// NEW(array2b);
	// *array2b = UArray2b_new(10, 10, sizeof(int), 3);
	// *array2b = UArray2b_new_64K_block(10, 10, sizeof(int));
	
	printf("array2b loc:\t%p\n", (void *)&array2b);
	printf("first elem :\t%p\n", methods->at(array2b, 0, 0));
	printf("blocksize  :\t%d\n", methods->blocksize(array2b));
	
	int *p;
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			p = methods->at(array2b, i, j);
			*p = (i * 10) + j;
	 	}
	}
	
	methods->map_block_major(array2b, apply_print, NULL);

	// for (int i=0; i<10; i++) {
	// 	for (int j=0; j<10; j++) {
	// 		p = UArray2b_at(*array2b, i, j);
	// 		printf("loc: %p\tval: %d\n", (void *)p, *p);
	// 	}
	// }

	methods->free(&array2b);
	// FREE(array2b);
	
	return 0;
}
