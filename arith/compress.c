#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "assert.h"

#include "trim_image.h"
#include "float_convert.h"
#include "colorspace_trans.h"
#include "to_averages.h"
#include "pack.h"
#include "output_compressed.h"
#include "input_compressed.h"

void compress(FILE *input) {
	// establish methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize ppm and read from input
	Pnm_ppm *image;
	NEW(image);
	*image = Pnm_ppmread(input, methods);
	
	// pull height, width and denominator from new ppm
	unsigned denom, width, height;
	height = (*image)->height;
	width  = (*image)->width;
	denom  = (*image)->denominator;
	(void)height; (void)width;
	
	// print ppm stats
	// printf("~~input image~~\nheight\t%d\nwidth\t%d\ndenom\t%d\n", height, width, denom);


	//
	// remove the right and bottom edge of the image if necesarry
	remove_edges(image);
	//
	//


	// pull the new width and height from the ppm
	unsigned new_height, new_width;
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
	
	
	// test	
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
	

	output(words, new_width, new_height);
	
	methods->free(averages);
	FREE(averages);

	methods->free(words);
	FREE(words);

	return;
}

void decompress(FILE *input) {
	// set methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize decompressed width and height
	unsigned width, height;
	width = 0;
	height = 0;
	
	// read header from compressed image file
	int read = fscanf(input, "Compressed image format 2\n%u %u", &width, &height);
	assert(read == 2);
	int c = getc(input);
	assert(c == '\n');
	
	//
	// initialize 2d array of 32 bit words and fill
	A2Methods_Array2 *words;
	words = input_collection(methods, input, width, height);
	/*uint64_t word;
	int words_width  = methods->width(*words);
	int words_height = methods->height(*words);
	for (int j = 0; j < words_height; j++) {
		for (int i = 0; i < words_width; i++) {
			printf("%d, %d", i, j);
			word = *(uint64_t *)methods->at(*words, i, j);
			printf("\t%" PRIx64 "\n", word);
		}
	}*/
	//
	//
	
	
	//
	// convert words to block components
	A2Methods_Array2 *averages;
	averages = unpack(methods, words);
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


	//
	//
	A2Methods_Array2 *colorspace;
	colorspace = decompose_averages(methods, averages, width, height);
	//
	//
	

	//
	//
	A2Methods_Array2 *floats;
	floats = from_colorspace(methods, colorspace, width, height);
	//
	//
	

	A2Methods_Array2 rgb = methods->new(width, height, sizeof(struct Pnm_rgb));
	struct Pnm_ppm pixmap = { .width = width, .height = height, .denominator = 255, .pixels = rgb, .methods = methods };


	//
	//
	from_float(methods, floats, &(pixmap.pixels), width, height, 255);
	//
	//
	
	Pnm_ppmwrite(stdout, &pixmap);
		
	
	methods->free(&rgb);
	// FREE(rgb);

	methods->free(floats);
	FREE(floats);

	methods->free(colorspace);
	FREE(colorspace);

	methods->free(words);
	FREE(words);

	methods->free(averages);
	FREE(averages);

	return;
}
