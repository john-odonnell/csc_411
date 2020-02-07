#include "uarray2.h"
#include "stdio.h"
#include "stdlib.h"
#include "mem.h"

void apply_add(void *p, int bit, void *cl){
	(void)cl;
	int *n = p;
	*n = bit;
	return;
}

void apply_print(void *p, int bit, void *cl){
	(void)cl;
	int *n = p;
	printf("idx: %d\tloc: %p\tval: %d\n", bit, p, *n);
	return;
}

int main(int argc, char *argv[]){
	(void)argc; (void)argv;

	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new(5, 5, sizeof(int));
	
	printf("height  : %d\n", UArray2_height(*uarray2));
	printf("width   : %d\n", UArray2_width(*uarray2));
	printf("size    : %d\n", UArray2_size(*uarray2));
	printf("arr loc0: %p\n\n", UArray2_at(*uarray2, 0, 0));

	UArray2_map_row_major(*uarray2, apply_add, NULL);

	printf("Checking map row_major for insertion and print:\n");
	UArray2_map_row_major(*uarray2, apply_print, NULL);

	UArray2_free(uarray2);
	free(uarray2);

	return 0;
}
