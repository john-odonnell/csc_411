#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "assert.h"
#include "uarray2.h"
#include "pnmrdr.h"

void fill_uarray2(UArray2_T uarray2, Pnmrdr_T puzzle){
	int n;
	int *r_location;
	printf("insertion start\n");
	printf("size: %d\n", uarray2->size);
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			n = Pnmrdr_get(puzzle);
			r_location = UArray2_at(uarray2, i, j);
			*r_location = n;
			printf("%d,%d: loc: %p\t%d\t%d\n", i, j, (void *)r_location, n, *r_location);
		}
	}
	printf("insertion complete\n");
	return;
}

void apply_print(void *p, int bit, void *cl){
	(void)bit;	(void)cl;
	int *l = p;
	printf("%d\n", *l);
	return;
}

int main(int argc, char *argv[]){
	assert(argc<=2);

	FILE *stream;
	if (argc==2){
		stream = fopen(argv[1],"r");
	}
	else{
		stream = stdin;
	}

	if (stream==NULL){
		fprintf(stderr, "%s: Could not open file %s for reading!\n", argv[0], argv[1]);
		exit(1);
	}

	Pnmrdr_T *puzzle;
       	NEW(puzzle);
	*puzzle = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(*puzzle);
	assert(data.width==9 && data.height==9 && data.denominator==9);

	UArray2_T *puzzle_R;
	NEW(puzzle_R);
	*puzzle_R = UArray2_new(9, 9, sizeof(int));
	UArray2_map_row_major(*puzzle_R, apply_print, NULL);
	
	printf("filling array\n");
	fill_uarray2(*puzzle_R, *puzzle);
	printf("array filled\n");
	fclose(stream);

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			int *val = UArray2_at(*puzzle_R, i, j);
			printf("%d,%d: loc: %p\t%d\n", i, j, (void *)val, *val);
		}
	}
		
	UArray2_map_row_major(*puzzle_R, apply_print, NULL);

	//if (check_solution(*puzzle_R)){
	//	exit(0);
	//}
	//else{
	//	exit(1);
	//}
	
	Pnmrdr_free(puzzle);
	free(puzzle);
	UArray2_free(puzzle_R);
	free(puzzle_R);	
	return(0);
}
