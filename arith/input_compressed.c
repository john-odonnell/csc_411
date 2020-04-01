#include <stdint.h>
#include <stdio.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"

#include "bitpack.h"

A2Methods_Array2 *input_collection(A2Methods_T methods, FILE *input, unsigned width, unsigned height) {
	//initialize 2d array of words and allocate
	A2Methods_Array2 *words;
	NEW(words);
	*words = methods->new(width/2, height/2, sizeof(uint64_t));

	uint64_t *destination;

	for (int j = 0; j < (int)height/2; j++) {
		for (int i = 0; i < (int)width/2; i++) {
			uint64_t codeword = 0;
			for (int k = 3; k >= 0; k--) {
				uint64_t byte = getc(input);
				codeword = Bitpack_newu(codeword, 8, k*8, byte);
			}

			destination = methods->at(*words, i, j);
			*destination = codeword;
		}
	}

	return words;
}
