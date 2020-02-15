#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mem.h"
#include "assert.h"
#include "uarray2.h"
#include "pnmrdr.h"
#include "set.h"

// fills a UArray2_T underlying array with value taken from the pnmreader
void fill_uarray2(UArray2_T uarray2, Pnmrdr_T pnm){
	int n;
	int *r_location;
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			// pull digit from pnm
			n = Pnmrdr_get(pnm);
			// check validity
			assert(n>0 && n<10);
			// pull memloc of corresponding i,j pair
			r_location = UArray2_at(uarray2, i, j);
			// store
			*r_location = n;	
		}
	}
	return;
}

// cmp function for storing integers in sets
int cmp(const void *x, const void *y){
	if(*(int *)x > *(int *)y)
		return 1;
	else if(*(int *)x < *(int *)y)
		return -1;
	else
		return 0;
}

// hash function for storing integers in sets
// bas hash but suitable for our purposes of using ints 1 through 9
unsigned hash(const void *x){
	return (*(int *)x);
}

// checks each column of the puzzle, compares it to base set {1,2,...,9}
bool check_columns(UArray2_T *puzzle, Set_T *base_set){
	for(int i=0; i<9; i++){
		// create new set representing integers found in column i
		Set_T column;
		column = Set_new(9, cmp, hash);
		// insert integers from column into set
		for(int j=0; j<9; j++){
			Set_put(column, (int *)UArray2_at(*puzzle, j, i));
		}
		
		// create new set representing the difference between column and base set
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

// checks each row of the puzzle and compares it to the base set {1,2,...,9}
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

// checks each box of the puzzle and compares it to the base set {1,2,...,9}
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

// checks if a given sudoku puzzle is a valid solution
bool check_solution(UArray2_T *puzzle){
	// initialize and assign base set to {1,2,...,9}
	Set_T *base_set;
	NEW(base_set);
	*base_set = Set_new(9, cmp, hash);
	int base[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(int i=0; i<9; i++){
		Set_put(*base_set, &base[i]);
	}
	// check all 9 columns, rows and boxes
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

	// open file/stdin
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
	
	// create new pnmreader and allocate
	Pnmrdr_T *pnm;
       	NEW(pnm);
	*pnm = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(*pnm);
	assert(data.width==9 && data.height==9 && data.denominator==9);
	
	// create new UArray2_T and allocate
	UArray2_T *puzzle;
	NEW(puzzle);
	*puzzle = UArray2_new(9, 9, sizeof(int));
	
	// fill 2d array from pnmreader
	fill_uarray2(*puzzle, *pnm);
	// free pnmreader, pointer, and close file stream
	Pnmrdr_free(pnm);
	free(pnm);
	fclose(stream);
	
	// exit conditions base on puzzle state
	if (check_solution(puzzle)){
		UArray2_free(puzzle);
		free(puzzle);
		// remoce/comment before submission
		exit(0);
	}
	else{
		UArray2_free(puzzle);
		free(puzzle);
		// remove/comment before submission
		exit(1);
	}

	return(0);
}
