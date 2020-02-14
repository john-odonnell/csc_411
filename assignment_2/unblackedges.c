#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "assert.h"
#include "bit2.h"
#include "pnmrdr.h"
#include "mem.h"

// fills a 2d bit array from a pnmreader
void fill_bit2(Bit2_T *bit2, Pnmrdr_T *pbm){
	int bit;
	for(int i=0; i<(Bit2_height(*bit2)); i++){
		for(int j=0; j<(Bit2_width(*bit2)); j++){
			bit = Pnmrdr_get(*pbm);
			assert(bit==0 || bit==1);
			Bit2_put(*bit2, i, j, bit);
		}
	}
	return;
}

// checks if any neighboring pixels are edge pixels
bool check_neighbors(Bit2_T *edge_pixels, int i, int j){
	if(Bit2_get(*edge_pixels, i-1, j)==1 || Bit2_get(*edge_pixels, i+1, j)==1){
		return true;
	}
	else if(Bit2_get(*edge_pixels, i, j-1)==1 || Bit2_get(*edge_pixels, i, j+1)==1){
		return true;
	}
	else{
		return false;
	}
}

// used to find yet undiscovered edge pixels next to a pixel recently found by edge_propogate_lvl1
// as there is no guarentee that the iterative loop will be able to find these again
void edge_propogate_lvl2(Bit2_T *bit2, Bit2_T *edge_pixels, int i, int j){
	if(Bit2_get(*bit2, i, j-1)==1 && Bit2_get(*edge_pixels, i, j-1)==0){
		Bit2_put(*edge_pixels, i, j-1, 1);
		edge_propogate_lvl2(bit2, edge_pixels, i, j-1);
	}
	if(Bit2_get(*bit2, i-1, j)==1 && Bit2_get(*edge_pixels, i-1, j)==0){
		Bit2_put(*edge_pixels, i-1, j, 1);
		edge_propogate_lvl2(bit2, edge_pixels, i-1, j);
	}
	if(Bit2_get(*bit2, i, j+1)==1 && Bit2_get(*edge_pixels, i, j+1)==0){
		Bit2_put(*edge_pixels, i, j+1, 1);
		edge_propogate_lvl2(bit2, edge_pixels, i, j+1);
	}

	return;
}

// used to find yet undiscovered edge pixels next to a recently found edge pixel
// no need to look in the j+1 direction, as that is up next in find_edge()
void edge_propogate_lvl1(Bit2_T *bit2, Bit2_T *edge_pixels, int i, int j){
	if(Bit2_get(*bit2, i, j-1)==1 && Bit2_get(*edge_pixels, i, j-1)==0){
		Bit2_put(*edge_pixels, i, j-1, 1);
		edge_propogate_lvl2(bit2, edge_pixels, i, j-1);
	}
	if(Bit2_get(*bit2, i-1, j)==1 && Bit2_get(*edge_pixels, i-1, j)==0){
		Bit2_put(*edge_pixels, i-1, j, 1);
		edge_propogate_lvl2(bit2, edge_pixels, i-1, j);
	}

	return;
}

// finds edge pixels, straightforward for those that are located on edges
// for other pixels, this function is assisted by recursive functions
// edge_propogate_lvl1 and edge_propogate_lvl2
void find_edge(Bit2_T *bit2, Bit2_T *edge_pixels){
	for(int i=0; i<(Bit2_height(*bit2)); i++){
		for(int j=0; j<(Bit2_width(*bit2)); j++){
			if(i==0 && Bit2_get(*bit2, i, j)==1){
				Bit2_put(*edge_pixels, i, j, 1);
			}
			else if(j==0 && Bit2_get(*bit2, i, j)==1){
				Bit2_put(*edge_pixels, i, j, 1);
			}
			else if(j==Bit2_width(*bit2)-1 && Bit2_get(*bit2, i, j)==1){
				Bit2_put(*edge_pixels, i, j, 1);
				edge_propogate_lvl1(bit2, edge_pixels, i, j);
			}
			else if(i==(Bit2_height(*bit2)-1) && Bit2_get(*bit2, i, j)==1){
				Bit2_put(*edge_pixels, i, j, 1);
				edge_propogate_lvl1(bit2, edge_pixels, i, j);
			}
			else if(Bit2_get(*bit2, i, j)==1 && check_neighbors(edge_pixels, i, j)){
				Bit2_put(*edge_pixels, i, j, 1);
				edge_propogate_lvl1(bit2, edge_pixels, i, j);
			}
		}
	}
	return;
}

// apply function for a bit2_map function
void apply_print(int n, int bit, void *cl){
	(void)cl;
	if(n%10==9){
		printf("%d\n", bit);
	}
	else{
		printf("%d ", bit);
	}
	return;
}

// outputs a given 2d bit array to a given file stream
void pbmwrite(FILE *stream, Bit2_T *bit2){
	stream = stdout;
	fputs("P1\n", stream);
	printf("%d %d", Bit2_width(*bit2), Bit2_height(*bit2));
	fputs("\n", stream);
	int bit;
	// change to use row major map function
	for(int i=0; i<Bit2_height(*bit2); i++){
		for(int j=0; j<Bit2_width(*bit2); j++){
			bit = Bit2_get(*bit2, i, j);
			if(j==(Bit2_width(*bit2)-1)){
				printf("%d\n", bit);
			}
			else{
				printf("%d ", bit);
			}
		}
	}
	return;
}

// removes the black edge pixels from the original image
void remove_edge(Bit2_T *bit2, Bit2_T *edge_pixels){
	int height = Bit2_height(*bit2);
	int width = Bit2_width(*bit2);
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			if(Bit2_get(*edge_pixels, i, j)==1){
				Bit2_put(*bit2, i, j, 0);
			}
		}
	}
	return;
}

void unblack(Bit2_T *bit2){
	// a 2d bit array representing which pixels are edge pixels
	// in order to determine which pixels are edge pixels 
	// by proximity
	Bit2_T *edge_pixels;
	NEW(edge_pixels);
	int height, width;
	height = Bit2_height(*bit2);
	width = Bit2_width(*bit2);
	*edge_pixels = Bit2_new(height, width);
	
	// find edge pixels in image and update edge pixel array
	find_edge(bit2, edge_pixels);
	
	// remove pixels and write to output
	remove_edge(bit2, edge_pixels);	
	pbmwrite(stdout, bit2);

	// free allocated memory
	Bit2_free(edge_pixels);
	free(edge_pixels);

	return;
}



int main(int argc, char *argv[]){
	assert(argc<=2);
	// open input file stream
	FILE *stream;
	if(argc==2){
		stream = fopen(argv[1],"r");
	}
	else{
		stream = stdin;
	}

	if(stream==NULL){
		fprintf(stderr, "%s: Could not open file %s.\n", argv[0], argv[1]);
		exit(1);
	}
	
	// established a new pnmreader
	Pnmrdr_T *pbm;
	NEW(pbm);
	*pbm = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(*pbm);
	assert(data.denominator==1);

	// creates a new 2d bit array to represent the input image
	Bit2_T *bit2;
	NEW(bit2);
	*bit2 = Bit2_new(data.height, data.width);

	// fills the bit array from input
	fill_bit2(bit2, pbm);
	fclose(stream);
	// removes black edges
	unblack(bit2);

	// free allocated memory
	Bit2_free(bit2);
	free(bit2);
	Pnmrdr_free(pbm);
	free(pbm);
	
	return 0;
}
