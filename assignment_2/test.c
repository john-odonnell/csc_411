#include "uarray2.h"
#include "stdio.h"
#include "stdlib.h"
#include "mem.h"
#include "bit2.h"

void apply_add(void *p, int bit, void *cl){
	(void)cl;
	int *n;
	n = p;
	*n = bit;
	return;
}

void apply_print(void *p, int bit, void *cl){
	(void)cl;
	int *n;
	n = p;
	printf("idx: %d\tloc: %p\tval: %d\n", bit, p, *n);
	return;
}

void test_uarray2(){
	int *to_print;
	
	printf("~~~UArray2~~~\n");
	UArray2_T *uarray2;
	NEW(uarray2);
	*uarray2 = UArray2_new(5, 5, (int)sizeof(int));
	
	printf("height  : %d\n", UArray2_height(*uarray2));
	printf("width   : %d\n", UArray2_width(*uarray2));
	printf("size    : %d\n", UArray2_size(*uarray2));

	to_print = UArray2_at(*uarray2, 0, 0);

	printf("arr loc0: %p\n", (void *)to_print++);
	printf("arr loc1: %p\n", (void *)to_print);
	printf("size int: %d\n\n", (int)sizeof(int));
	UArray2_map_row_major(*uarray2, apply_add, NULL);

	printf("Checking map row_major for insertion and print:\n");
	UArray2_map_row_major(*uarray2, apply_print, NULL);
	
	to_print = UArray2_at(*uarray2, 0, 0);
	for(int i = 0; i<25; i++){
		printf("val arr%d: %d\n", i, *to_print);
		to_print++;
	}
	
	UArray2_free(uarray2);
	free(uarray2);

	return;
}

void test_bit2(){
	printf("~~Bit2~~~\n");
	Bit2_T *bit2;
	NEW(bit2);
	*bit2 = Bit2_new(5, 5);

	printf("height : %d\n", Bit2_height(*bit2));
	printf("width  : %d\n", Bit2_width(*bit2));
	
	int bit = 1;
	for(int i = 0; i<5; i++){
		for(int j = 0; j<5; j++){
			Bit2_put(*bit2, i, j, bit);
			bit = (int)(!(bit && 1));
		}
	}
	for(int i = 0; i<5; i++){
		for(int j = 0; j<5; j++){
			int to_print = Bit2_get(*bit2, i, j);
			printf("%d,%d: %d\n", i, j, to_print);
		}
	}

	Bit2_free(bit2);
	free(bit2);

	return;

}

int main(int argc, char *argv[]){
	(void)argc; (void)argv;

	test_uarray2();
	printf("\n");
	test_bit2();

	return 0;
}
