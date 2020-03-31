#include <stdio.h>
#include <math.h>

#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "to_averages.h"
#include "colorspace_trans.h"
#include "arith411.h"

float maintain_range(float x) {
	if (x < -0.3) {
		return -0.3;
	} else if (x > 0.3) {
		return 0.3;
	} else {
		return x;
	}
}

A2Methods_Array2 *get_averages(A2Methods_Array2 *arr, int width, int height) {
	// set methods
	A2Methods_T methods = array2_methods_plain;
	
	// initialize array of 2x2 blocks
	A2Methods_Array2 *blocks;
       	NEW(blocks);
	*blocks	= methods->new(width/2, height/2, sizeof(struct block_components));
	
	// initialize lum values and
	// Pnm_colorspace pointer
	float    Y1,    Y2, Y3, Y4;
	float     a,     b,  c,  d;
	float totPb, totPr;
	Pnm_colorspace this_pix;

	totPb = 0;
	totPr = 0;
	
	block_components this_block;

	// for each block
	for (int b_i = 0; b_i < width; b_i+=2) {
		for (int b_j = 0; b_j < height; b_j+=2) {
			// for each cell in the block
			for (int i = b_i; i < b_i+2; i++) {
				for (int j = b_j; j < b_j+2; j++) {
					// pulll memory location of colorspace struct
					this_pix = methods->at(*arr, i, j);

					// assign Y based on location
					if (i == b_i && j == b_j) {
						Y1 = this_pix->Y;
					} else if (i == b_i+1 && j == b_j) {
						Y2 = this_pix->Y;
					} else if (i == b_i && j == b_j+1) {
						Y3 = this_pix->Y;
					} else {
						Y4 = this_pix->Y;
					}

					// add to total Pb and Pr
					totPb += this_pix->Pb;
					totPr += this_pix->Pr;
				}
			}
			
			this_block = methods->at(*blocks, b_i/2, b_j/2);
			this_block->avgPb = Arith_index_of_chroma(totPb/4.0);
			this_block->avgPr = Arith_index_of_chroma(totPr/4.0);

			a = (Y4 + Y3 + Y2 + Y1)/4.0;
			b = (Y4 + Y3 - Y2 - Y1)/4.0;
			c = (Y4 - Y3 + Y2 - Y1)/4.0;
			d = (Y4 - Y3 - Y2 + Y1)/4.0;

			b = maintain_range(b);
			c = maintain_range(c);
			d = maintain_range(d);

			this_block->a = (unsigned)(roundf(a * 511));
			this_block->b =   (signed)(roundf(b *  50));
			this_block->c =   (signed)(roundf(c *  50));
			this_block->d =   (signed)(roundf(d *  50));
		}
	}

	return blocks;
}
