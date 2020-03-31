#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"

#include "trim_image.h"
#include "float_convert.h"
#include "colorspace_trans.h"
#include "to_averages.h"
#include "pack.h"
#include "output_compressed.h"

void compress(FILE *input) {
	// establish methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize ppm and read from input
	Pnm_ppm *image;
	NEW(image);
	*image = Pnm_ppmread(input, methods);
	
	// pull height, width and denominator from new ppm
	int denom; // width, denom;
	// height = (*image)->height;
	// width  = (*image)->width;
	denom  = (*image)->denominator;
	
	// print ppm stats
	// printf("~~input image~~\nheight\t%d\nwidth\t%d\ndenom\t%d\n", height, width, denom);


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
	// printf("~~trimmed~~\nheight\t%d\nwidth\t%d\ndenom\t%d\n", new_height, new_width, denom);

	// initialize new 2d array
	A2Methods_Array2 *altered;


	//
	// convert to float rgb structs
	altered = to_float(image, new_width, new_height, denom);
	// free ppm and pointer
	Pnm_ppmfree(image);
	FREE(image);
	//
	//


	//
	// convert to component video
	to_colorspace(altered, new_width, new_height);
	//
	//


	//
	// compute averages
	A2Methods_Array2 *averages;
	averages = get_averages(altered, new_width, new_height);
	// free altered and pointer
	methods->free(altered);
	FREE(altered);
	//
	//
	

	//
	// pack averages into words
	A2Methods_Array2 *words;
	words = pack(averages);
	// free averages and pointer
	//
	//
	
	/*int words_i, words_j;
	words_i = methods->width(*words);
	words_j = methods->height(*words);
	uint64_t *this_word;
	block_components this_block;

	for (int i = 0; i < words_i; i++) {
		for (int j = 0; j < words_j; j++) {
			printf("i\t%d\tj\t%d\n", i, j);
			this_word  = methods->at(*words, i, j);
			this_block = methods->at(*averages, i, j);

			printf("\t%" PRIx64 "\n", *this_word);
			printf("\ta\t%u\n", this_block->a);
			printf("\tb\t%d\n", this_block->b);
			printf("\tc\t%d\n", this_block->c);
			printf("\td\t%d\n", this_block->d);
			printf("\tPb\t%u\n", this_block->avgPb);
			printf("\tPr\t%u\n", this_block->avgPr);
		}
	}*/	
	

	output(words);
	
	methods->free(averages);
	FREE(averages);

	methods->free(words);
	FREE(words);

	return;
}
