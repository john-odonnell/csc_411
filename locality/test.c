#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "uarray2b.h"

void apply_print (int i, int j, UArray2b_T array2b, void *elem, void *cl) {
	(void)array2b; (void)cl;
	printf("i: %d\tj: %d\tloc: %p\tval: %d\n", i, j, elem, *(int *)elem);
}

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;

	UArray2b_T *array2b;
	NEW(array2b);
	*array2b = UArray2b_new(10, 10, sizeof(int), 3);
	// *array2b = UArray2b_new_64K_block(10, 10, sizeof(int));
	printf("array2b loc:\t%p\n", (void *)array2b);
	printf("first elem :\t%p\n", UArray2b_at(*array2b, 0, 0));
	printf("blocksize  :\t%d\n", UArray2b_blocksize(*array2b));
	
	int *p;
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			p = UArray2b_at(*array2b, i, j);
			*p = (i * 10) + j;
	 	}
	}
	
	UArray2b_map(*array2b, apply_print, NULL);

	// for (int i=0; i<10; i++) {
	// 	for (int j=0; j<10; j++) {
	// 		p = UArray2b_at(*array2b, i, j);
	// 		printf("loc: %p\tval: %d\n", (void *)p, *p);
	// 	}
	// }

	UArray2b_free(array2b);
	FREE(array2b);
	
	return 0;
}
