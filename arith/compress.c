#include <stdio.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"

#include "trim_image.h"
#include "float_convert.h"
#include "colorspace_trans.h"

void compress(FILE *input) {
	// establish methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize ppm and read from input
	Pnm_ppm *image;
	NEW(image);
	*image = Pnm_ppmread(input, methods);
	
	// pull height, width and denominator from new ppm
	int height, width, denom;
	height = (*image)->height;
	width  = (*image)->width;
	denom  = (*image)->denominator;
	
	// print ppm stats
	printf("~~input image~~\nheight\t%d\nwidth\t%d\ndenom\t%d\n", height, width, denom);


	//
	// remove the right and bottom edge of the image if necesarry
	remove_edges(image);
	//
	//


	// pull the new width and height from the ppm
	int new_height, new_width;
	new_height = (*image)->height;
	new_width  = (*image)->width;
	
	// print ppm stats
	printf("~~trimmed~~\nheight\t%d\nwidth\t%d\ndenom\t%d\n", new_height, new_width, denom);

	// initialize new 2d array
	A2Methods_Array2 *altered;


	//
	// convert to float rgb structs
	altered = to_float(image, new_width, new_height, denom);
	//
	//
	
	
	//
	// convert to component video
	to_colorspace(altered, new_width, new_height);
	//
	//


	// free ppm and pointer
	Pnm_ppmfree(image);
	FREE(image);
	
	// free 2d array and pointer
	methods->free(altered);
	FREE(altered);

	return;
}
