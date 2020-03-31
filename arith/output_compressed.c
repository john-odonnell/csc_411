#include <stdio.h>
#include <stdint.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "bitpack.h"

void output(A2Methods_Array2 *arr) {
	A2Methods_T methods = array2_methods_plain;

	int width = methods->width(*arr);
	int height = methods->height(*arr);
	
	printf("Compressed image format 2\n%u %u", width, height);
	printf("\n");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			// printf("%d,%d\n", i, j);
			
			uint64_t codeword = *(uint64_t *)methods->at(*arr, i, j);
			for (int k = 3; k >= 0; k--) {
				// printf("%d  ", k);
				char byte = (char)Bitpack_getu(codeword, 8, (k * 8));
				putchar(byte);
				// printf("\t");
			}

			// printf("\n");
		}
	}
	printf("\n");

	return;
}
