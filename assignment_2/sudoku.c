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
		}
	}
	return;
}

int cmp(const void *x, const void *y){
	if(*(int *)x > *(int *)y)
		return 1;
	else if(*(int *)x < *(int *)y)
		return -1;
	else
		return 0;
}

unsigned hash(const void *x){
	return (*(int *)x);
}

bool check_columns(UArray2_T *puzzle, Set_T *base_set){
	for(int i=0; i<9; i++){
		Set_T column;
		column = Set_new(9, cmp, hash);
		for(int j=0; j<9; j++){
			Set_put(column, (int *)UArray2_at(*puzzle, j, i));
		}
		
		Set_T diff;
		diff = Set_diff(*base_set, column);

		if(Set_length(diff) != 0){
			Set_free(&diff);
			Set_free(&column);
			return false;
		}
		
		Set_free(&diff);
		Set_free(&column);
	}
	return true;
}

bool check_rows(UArray2_T *puzzle, Set_T *base_set){
	for(int i=0; i<9; i++){
		Set_T row;
		row = Set_new(9, cmp, hash);
		for(int j=0; j<9; j++){
			Set_put(row, (int *)UArray2_at(*puzzle, i, j));
		}

		Set_T diff;
		diff = Set_diff(*base_set, row);

		if(Set_length(diff) != 0){
			Set_free(&diff);
			Set_free(&row);
			return false;
		}

		Set_free(&diff);
		Set_free(&row);
	}
	return true;
}

bool check_boxes(UArray2_T *puzzle, Set_T *base_set){
	for(int i=0; i<9; i+=3){
		for(int j=0; j<9; j+=3){
			Set_T box;
			box = Set_new(9, cmp, hash);
			for(int k=i; k<i+3; k++){
				for(int m=j; m<j+3; m++){
					Set_put(box, (int *)UArray2_at(*puzzle, k, m));
				}
			}
			
			Set_T diff;
			diff = Set_diff(*base_set, box);

			if(Set_length(diff) != 0){
				Set_free(&diff);
				Set_free(&box);
				return false;
			}

			Set_free(&diff);
			Set_free(&box);
		}
	}
	return true;
}

void set_apply(const void *member, void *cl){
	(void)cl;
	int *n;
	n = (int *)member;
	printf("%d\n", *n);
	return;
}

bool check_solution(UArray2_T *puzzle){
	Set_T *base_set;
	NEW(base_set);
	*base_set = Set_new(9, cmp, hash);
	
	int base[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(int i=0; i<9; i++){
		Set_put(*base_set, &base[i]);
	} // WHERE I LEFT OFF
		// COMPARING EACH ROW/COLUMN/BOX TO THE BASE SET
		// A LOT EASIER THAN ITERATING THROUGH THE ENTIRE PUZZLE
		// MANY TIMES
	
	// printf("Base set:\t");	
	// Set_map(*base_set, set_apply, NULL);

	if(check_columns(puzzle, base_set) && check_rows(puzzle, base_set) && check_boxes(puzzle, base_set)){
		Set_free(base_set);
		free(base_set);
		return true;
	}
	else{
		Set_free(base_set);
		free(base_set);
		return false;
	}
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

	if (check_solution(puzzle)){
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
