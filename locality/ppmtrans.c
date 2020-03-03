#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "mem.h"

// closure structure
struct a2closure {
	A2Methods_applyfun *apply;	// pointer to apply function
	A2Methods_Array2 *original;	// pointer to original array
	A2Methods_Array2 *rotated;	// pointer to rotated array
	A2Methods_T methods;		// methods
	int rotation;			// rotation
};

// for each element in the original array
void apply_trans(int i, int j, A2Methods_Array2 original, void *elem, void *cl) {
 	(void) original;
	
	// cast void *cl as a a2closure struct
	struct a2closure *mycl = (struct a2closure *)cl;
	
	// save closure members for easy access
	int rotation = (*mycl).rotation;
	A2Methods_T methods = (*mycl).methods;
	A2Methods_Array2 *rotated = (*mycl).rotated;
	
	// typecast void *elem as pnm_rgb *
	struct Pnm_rgb *orig = (struct Pnm_rgb *)elem;
	
	// save new_height and width for easy access
	int new_height, new_width;
	new_height = methods->height(*rotated);
	new_width = methods->width(*rotated);
	
	// calculate new i and j based on rotation
	int new_i, new_j;
	if (rotation==90) {
		new_i = new_width-j-1;
		new_j = i;
	} else if (rotation==180) {
		new_i = new_width-i-1;
		new_j = new_height-j-1;
	} else if (rotation==270) {
		new_i = j;
		new_j = new_height-i-1;
	}

	// create pointer to the correct location in the rotated array
	struct Pnm_rgb *new = (struct Pnm_rgb *)methods->at(*rotated, new_i, new_j);

	// assign the value in the original array to the new array
	*new = *orig;

	return;
}

void rotate(A2Methods_Array2 *rotated, A2Methods_Array2 *original, A2Methods_T methods, int rotation){
	
	// closure for changing rotated array while mapping
	struct a2closure mycl = {apply_trans, original, rotated, methods, rotation};
	methods->map_default(*original, apply_trans, &mycl);
	
	return;	
}

void transform_ppm(FILE *stream, A2Methods_T methods, int rotation, A2Methods_mapfun map, char mapping[]){
	(void) map;
	
	// new ppm, read from input stream
	Pnm_ppm *ppm;
	NEW(ppm);
	*ppm = Pnm_ppmread(stream, methods);
	fclose(stream);
	
	// pull stuct members from ppm to define rotated pppm
	int width = (*ppm)->width;
	int height = (*ppm)->height;
	int denominator = (*ppm)->denominator;
	(void) denominator;
	A2Methods_Array2 pixels = (*ppm)->pixels;
	int size = methods->size(pixels);
	
	// set up rotated array2 of opposite height/width
	A2Methods_Array2 *rotated;
	NEW(rotated);
	if (rotation==90 || rotation==270) {
		*rotated = methods->new(height, width, size);
	} else {
		*rotated = methods->new(width, height, size);
	}
	
	time_t start, end;
	clock_t cpu_start, cpu_end;

	// if the image is to be rotated, rotate it
	if (rotation != 0) {
		start = time(NULL);
		cpu_start = clock();
		rotate(rotated, &pixels, methods, rotation);
		cpu_end = clock();
		end = time(NULL);
	}

	time_t elapsed = end - start;
	clock_t cpu_cycles = cpu_end - cpu_start;

	FILE *times = fopen("times.txt","a");
	fprintf(times, "~~~%d deg rotation with %s~~~\n", rotation, mapping); 
	fprintf(times, "time in seconds: %ld\n", elapsed);
	fprintf(times, "clock cycles   : %ld\n", cpu_cycles);
	fprintf(times, "\n\n");
	fclose(times);
	
	// change the struct members to the new rotated values
	if (rotation == 90 || rotation == 270) {
		(*ppm)->width = height;
		(*ppm)->height = width;
		(*ppm)->pixels = *rotated;
	} else if (rotation == 180) {
		(*ppm)->pixels = *rotated;
	}
	
	// write ppm to standard output
	Pnm_ppmwrite(stdout, *ppm);
	
	// free ppm and pointer
	if (rotation == 0) {
		methods->free(rotated);
	} else {
		methods->free(&pixels);
	}
	FREE(rotated);
	Pnm_ppmfree(ppm);
	FREE(ppm);

	return;
}

int main(int argc, char *argv[]) {
  	// rotation angle and mapping style
	int rotation = 0;
  	char mapping[13];

  	A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
  	assert(methods);
  
  	A2Methods_mapfun *map = methods->map_default; // default to best map
  	assert(map);

  	FILE *stream;
  	if (argc == 5) {
		stream = fopen(argv[4],"r");
  	} else {
  	        stream = stdin;
  	}

  	if (stream==NULL) {
  	        fprintf(stderr, "%s: Could not open file %s.\n", argv[0], argv[3]);
  	}

  	#define SET_METHODS(METHODS, MAP, WHAT) do { \
  	    	methods = (METHODS); \
  	    	assert(methods); \
  	    	map = methods->MAP; \
  	    	if (!map) { \
  	      		fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
  	      		exit(1); \
  	    	} \
  	} while(0)

  	int i;
  	for (i = 1; i < argc; i++) {
  	  	if (!strcmp(argv[i], "-row-major")) {
  	    		SET_METHODS(array2_methods_plain, map_row_major, "row-major");
  	    		strcpy(mapping, "-row-major");
  	  	} else if (!strcmp(argv[i], "-col-major")) {
  	    		SET_METHODS(array2_methods_plain, map_col_major, "column-major");
  	    		strcpy(mapping, "-col-major");
  	  	} else if (!strcmp(argv[i], "-block-major")) {
  	    		SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
  	    		strcpy(mapping, "-block-major");
  	  	} else if (!strcmp(argv[i], "-rotate")) {
  	    		assert(i + 1 < argc);
  	    		char *endptr;
  	    		rotation = strtol(argv[++i], &endptr, 10);
  	    		assert(*endptr == '\0'); // parsed all correctly
  	    		assert(rotation == 0   || rotation == 90
  	        		|| rotation == 180 || rotation == 270);
  	  	} else if (!strcmp(argv[i], "-flip")) {
  	      		fprintf(stderr, "%s: '%s' not implemented\n", argv[0], argv[i]);
  	      		exit(1);
  	  	} else if (!strcmp(argv[i], "-transpose")) {
  	          	fprintf(stderr, "%s: '%s' not implemented\n", argv[0], argv[i]);
  	          	exit(1);
  	  	} else if (*argv[i] == '-') {
  	    		fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
  	    		exit(1);
  	  	} else if (argc - i > 2) {
  	    		fprintf(stderr, "Usage: %s [-rotate <angle>] "
  	            		"[-{row,col,block}-major] [filename]\n", argv[0]);
  	    		exit(1);
  	  	} else {
  	    		break;
  	  	}
  	}
	
	transform_ppm(stream, methods, rotation, map, mapping);
}

#undef A2Arr
