#include "stdint.h"

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "to_averages.h"
#include "bitpack.h"

A2Methods_Array2 *pack(A2Methods_Array2 *arr) {
	// set methods
	A2Methods_T methods = array2_methods_plain;

	int width  = methods->width(*arr);
	int height = methods->height(*arr);

	A2Methods_Array2 *words;
	NEW(words);
	*words = methods->new(width, height, sizeof(uint64_t));

	block_components this_block;
	uint64_t *word_dest;
	uint64_t codeword;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			codeword = 0;

			this_block = methods->at(*arr, i, j);
			codeword = Bitpack_newu(codeword, 9, 23, this_block->a);
			codeword = Bitpack_news(codeword, 5, 18, this_block->b);
			codeword = Bitpack_news(codeword, 5, 13, this_block->c);
			codeword = Bitpack_news(codeword, 5,  8, this_block->d);
			codeword = Bitpack_newu(codeword, 4,  4, this_block->avgPb);
			codeword = Bitpack_newu(codeword, 4,  0, this_block->avgPr);

			word_dest = methods->at(*words, i, j);
			*word_dest = codeword;
		}
	}

	return words;
}
