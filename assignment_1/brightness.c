#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "assert.h"
#include "pnmrdr.h"

// float brightness(Pnmrdr_T pnm, int pxl_count, int denom) {
//	float avg;
//	for(int i = 0; i < pxl_count; i++) {
//		
//	}
// }

int main(int argc, char* argv[]) {
	assert(argc<=2);
	
	// open file
	FILE *image;
	// if the filename is given as a command line argument
	if (argc>1) {
		image = fopen(argv[1],"r");
	}
	// if the file is given through std input
	else {
		image = stdin;
	}

	// kick error if file not found
	if (image == NULL) {
		fprintf(stderr, "%s: Could not open file %s for reading!\n",
			       	argv[0], argv[1]);
		exit(1);
	}

	// create pnmrdr and gather mapdata
	Pnmrdr_T pnm = Pnmrdr_new(image);
	Pnmrdr_mapdata data = Pnmrdr_data(pnm);
	unsigned width = data.width;
	unsigned height = data.height;
	unsigned denom = data.denominator;

	// print results
	printf("File opened\n");
	printf("argv[1]: %s\n", argv[1]);

	printf("width: %d\n", width);
	printf("height: %d\n", height);
	printf("denom: %d\n", denom);
	
	// close the file stream and pnmreader
	fclose(image);
	Pnmrdr_free(&pnm);

	return 0;
}
