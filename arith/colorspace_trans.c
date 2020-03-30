#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "float_convert.h"
#include "colorspace_trans.h"

void to_colorspace(A2Methods_Array2 *arr, int width, int height) {
	// set methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize float variables
	float red, green, blue;
	float   Y,    Pb,   Pr;
	
	// establish void pointer for inter-type assignment
	Pnm_rgb_float this_pix_f;
	Pnm_colorspace destination;

	// for each rgb float set in the array
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
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
