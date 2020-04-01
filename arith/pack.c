#include <stdint.h>
#include <stdio.h>

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

A2Methods_Array2 *unpack(A2Methods_T methods, A2Methods_Array2 *words) {
	int width  = methods->width(*words);
	int height = methods->height(*words);

	// initialize 2d array of 32 bit words and allocate on the heap
	A2Methods_Array2 *blocks;
	NEW(blocks);
	*blocks = methods->new(width, height, sizeof(struct block_components));

	uint64_t codeword;
	block_components destination;

	unsigned a, avgPb, avgPr;
	signed   b,     c,     d;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			codeword = *(uint64_t *)methods->at(*words, i, j);

			a     = (unsigned)Bitpack_getu(codeword, 9, 23);
			b     =   (signed)Bitpack_gets(codeword, 5, 18);
			c     =   (signed)Bitpack_gets(codeword, 5, 13);
			d     =   (signed)Bitpack_gets(codeword, 5,  8);
			avgPb = (unsigned)Bitpack_getu(codeword, 4,  4);
			avgPr = (unsigned)Bitpack_getu(codeword, 4,  0);

			destination = methods->at(*blocks, i, j);

			destination->a     = a;
			destination->b     = b;
			destination->c     = c;
			destination->d     = d;
			destination->avgPb = avgPb;
			destination->avgPr = avgPr;
		}
	}

	return blocks;
}
