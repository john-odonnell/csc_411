#include <stdio.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"

#include "float_convert.h"
#include "colorspace_trans.h"

void to_colorspace(A2Methods_Array2 *arr, unsigned width, unsigned height) {
	// set methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize float variables
	float red, green, blue;
	float   Y,    Pb,   Pr;
	
	// establish void pointer for inter-type assignment
	Pnm_rgb_float this_pix_f;
	Pnm_colorspace destination;

	// for each rgb float set in the array
	for (int j = 0; j < (int)height; j++) {
		for (int i = 0; i < (int)width; i++) {
			// pull memory location of pixel
			this_pix_f = methods->at(*arr, i, j);

			// pull rgb
			red   = this_pix_f->red;
			green = this_pix_f->green;
			blue  = this_pix_f->blue;

			// set Y/Pb/Pr
			Y  =     (0.299 * red) +    (0.587 * green) +    (0.114 * blue);
			Pb = (-0.168736 * red) - (0.331264 * green) +      (0.5 * blue);
			Pr =       (0.5 * red) - (0.418688 * green) - (0.081312 * blue);

			// set destination
			destination = methods->at(*arr, i, j);
			destination->Y  = Y;
			destination->Pb = Pb;
			destination->Pr = Pr;
		}
	}

	return;
}

A2Methods_Array2 *from_colorspace(A2Methods_T methods, A2Methods_Array2 *colorspace, unsigned width, unsigned height) {
	float red, green, blue;
	float   Y,    Pb,   Pr;

	Pnm_colorspace this_pix;
	Pnm_rgb_float destination;

	A2Methods_Array2 *floats;
	NEW(floats);
	*floats = methods->new((int)width, (int)height, sizeof(struct Pnm_rgb_float));

	for (int j = 0; j < (int)height; j++) {
		for (int i = 0; i < (int)width; i++) {
			this_pix = methods->at(*colorspace, i, j);

			Y  = this_pix->Y;
			Pb = this_pix->Pb;
			Pr = this_pix->Pr;

			red   = (1.0 * Y) +      (0.0 * Pb) +    (1.402 * Pr);
			green = (1.0 * Y) - (0.344136 * Pb) - (0.714136 * Pr);
			blue  = (1.0 * Y) +    (1.772 * Pb) +      (0.0 * Pr);

			destination = methods->at(*floats, i, j);
			destination->red   = red;
			destination->green = green;
			destination->blue  = blue;


/*			printf("%d,%d\n", i, j);
			printf("\tY :%f\n",   Y);
			printf("\tPb:%f\n",  Pb);
			printf("\tPr:%f\n",  Pr);
			printf("\t r:%f\n", red);
			printf("\t g:%f\n", green);
			printf("\t b:%f\n", blue);
*/		}
	}

	return floats;
}
