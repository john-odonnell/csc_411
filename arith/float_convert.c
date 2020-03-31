#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "pnm.h"
#include "float_convert.h"

A2Methods_Array2 *to_float(Pnm_ppm *image, int width, int height, int denom) {
	// set methods
	A2Methods_T methods = array2_methods_plain;	
	
	// instantiate new 2d array for float structs
	A2Methods_Array2 *arr;
	NEW(arr);
	*arr = methods->new(width, height, sizeof(struct Pnm_rgb_float));
	
	// initialize rgb variables
	int    red,  green,  blue;
	float redf, greenf, bluef, denomf;
	
	// convert denominator to floating point
	denomf = (float)denom;
	
	// pointers for rgb pixels and float struct destinations
	Pnm_rgb this_pix_u;
	Pnm_rgb_float destination;
	
	// for each rgb set in the array
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			// pull memory location of pixel
			this_pix_u = methods->at((*image)->pixels, i, j);
			
			// pull rgb
			red   = this_pix_u->red;
			green = this_pix_u->green;
			blue  = this_pix_u->blue;

			// create floating point rgb
			redf   = (float)red   / denomf;
			greenf = (float)green / denomf;
			bluef  = (float)blue  / denomf;
			
			// find destination in new array and assign
			destination = methods->at(*arr, i, j);
			destination->red = redf;
			destination->green = greenf;
			destination->blue = bluef;
		}
	}

	return arr;
}