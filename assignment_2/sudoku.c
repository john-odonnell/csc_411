#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mem.h"
#include "assert.h"
#include "uarray2.h"
#include "pnmrdr.h"
#include "set.h"

//int cmp(const void *x, const void *y){
//	if(*(int *)x > *(int *)y)
//		return 1;
//	else if(*(int *)x < *(int *)y)
//		return -1;
//	else
//		return 0;
//}
//
//unsigned hash(const void *x){
//	return *(int *)x;
//}

void fill_uarray2(UArray2_T uarray2, Pnmrdr_T pnm){
	int n;
	int *r_location;
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			n = Pnmrdr_get(pnm);
			if(n<1 || n>9){
				exit(1);
			}
			r_location = UArray2_at(uarray2, i, j);
			*r_location = n;
			printf("%d,%d: loc: %p\t%d\t%d\n", i, j, (void *)r_location, n, *r_location);
		}
	}
	return;
}

bool check_rows(UArray2_T puzzle){
	for(int i=0; i<9; i++){
		for(int j=0; j<8; j++){
			int *to_check = UArray2_at(puzzle, i, j);
			for(int k=j+1; k<9; k++){
				int *check_against = UArray2_at(puzzle, i, k);
				if(*to_check == *check_against)
					return false;
			}
		}
	}
	return true;
}

bool check_columns(UArray2_T puzzle){
	for(int i=0; i<9; i++){
		for(int j=0; j<8; j++){
			int *to_check = UArray2_at(puzzle, i, j);
			for(int k=j+1; k<9; k++){
				int *check_against = UArray2_at(puzzle, i, k);
				if(*to_check == *check_against)
					return false;
			}
		}
	}
	return true;
}

bool check_solution(UArray2_T puzzle){
	// Set_T *base_set;
	// NEW(base_set);
	// *base_set = Set_new(9, cmp, hash);

	if(check_columns(puzzle) && check_rows(puzzle)) // && check_boxes(puzzle))
		return true;
	else
		return false;
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

	Pnmrdr_T *pnm;
       	NEW(pnm);
	*pnm = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(*pnm);
	assert(data.width==9 && data.height==9 && data.denominator==9);

	UArray2_T *puzzle;
	NEW(puzzle);
	*puzzle = UArray2_new(9, 9, sizeof(int));
		
	fill_uarray2(*puzzle, *pnm);
	Pnmrdr_free(pnm);
	free(pnm);
	fclose(stream);

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			int *val = UArray2_at(*puzzle, i, j);
			printf("%d,%d: loc: %p\t%d\n", i, j, (void *)val, *val);
		}
	}
		
	UArray2_map_row_major(*puzzle, apply_print, NULL);

	if (check_solution(*puzzle)){
		UArray2_free(puzzle);
		free(puzzle);
		printf("Good Solution\n");
		exit(0);
	}
	else{
		UArray2_free(puzzle);
		free(puzzle);
		printf("Bad Solution\n");
		exit(1);
	}

	return(0);
}
