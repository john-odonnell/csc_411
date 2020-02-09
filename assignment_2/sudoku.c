#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "assert.h"
#include "uarray2.h"
#include "pnmrdr.h"

void fill_uarray2(UArray2_T uarray2, Pnmrdr_T puzzle){
	int n;
	int *r_location;

	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			n = Pnmrdr_get(puzzle);
			r_location = intUArray2_at(uarray2, i, j);
			*r_location = n;
		}
	}
	return;
}

bool checkSolution(UArray2_T uarray2){
	
}

int main(int argc, char *argv){
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

	Pnmrdr_T puzzle = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(puzzle);

	UArray2_T *puzzle_R;
	NEW(puzzle_R);
	*puzzle_R = UArray2_new(9, 9, sizeof(int));

	full_uarray(*puzzle_R, puzzle);
	fclose(stream);
	Pnmrdr_free(&puzzle);

	if (check_solution(*puzzle_R)){
		exit(0);
	}
	else{
		exit(1);
	}	
}
