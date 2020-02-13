#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "bit2.h"
#include "pnmrdr.h"
#include "mem.h"

void fill_bit2(Bit2_T *bit2, Pnmrdr_T *pbm){
	int bit;
	for(int i=0; i<(Bit2_height(*bit2)); i++){
		for(int j=0; j<(Bit2_width(*bit2)); j++){
			bit = Pnmrdr_get(*pbm);
			assert(bit==0 || bit==1);
			Bit2_put(*bit2, i, j, bit);
			// printf("(%d, %d):\t%d\n", i, j, bit);
		}
	}
	return;
}

void _unblack_recurse(Bit2_T *bit2, int i, int j){
	// printf("(%d, %d)\t%d\n", i, j, Bit2_get(*bit2, i, j));
	if(Bit2_get(*bit2, i, j)==0){
		return;
	}
	else{
		Bit2_put(*bit2, i, j, 0);

		if(j<(Bit2_width(*bit2)-1)){
			_unblack_recurse(bit2, i, j+1);
		}
		if(i<(Bit2_height(*bit2)-1)){
			_unblack_recurse(bit2, i+1, j);
		}
		if(j>0){
			_unblack_recurse(bit2, i, j-1);
		}
		if(i>0){
			_unblack_recurse(bit2, i-1, j);
		}

		return;
	}
}

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

void pbmwrite(FILE *stream, Bit2_T *bit2){
	stream = stdout;
	fputs("P1\n", stream);
	printf("%d %d", Bit2_width(*bit2), Bit2_height(*bit2));
	fputs("\n", stream);
	int bit;
	for(int i=0; i<Bit2_height(*bit2); i++){
		for(int j=0; j<Bit2_width(*bit2); j++){
			bit = Bit2_get(*bit2, i, j);
			if(j==(Bit2_width(*bit2)-1)){
				printf("%d\n", bit);
				// fputs("\n", stream);
			}
			else{
				printf("%d ", bit);
				// fputs(" ", stream);
			}
		}
	}
	return;
}

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
	// for(int i=0; i<(Bit2_width(*bit2)); i++){
	// 	_unblack_recurse(bit2, 0, i);
	// 	_unblack_recurse(bit2, (Bit2_height(*bit2)-1), i);
	// }
	// for(int j=0; j<(Bit2_height(*bit2)); j++){
	// 	_unblack_recurse(bit2, j, 0);
	// 	_unblack_recurse(bit2, j, (Bit2_width(*bit2)-1));
	// }
	
	Bit2_T *edge_pixels;
	NEW(edge_pixels);
	int height, width;
	height = Bit2_height(*bit2);
	width = Bit2_width(*bit2);
	*edge_pixels = Bit2_new(height, width);

	find_edge(bit2, edge_pixels);
	// fix_edges(bit2, edge_pixels);
	// pbmwrite(stdout, edge_pixels);
	// pbmwrite(stdout, bit2);
	
	remove_edge(bit2, edge_pixels);	
	pbmwrite(stdout, bit2);

	// printf("Original:\n");
	// Bit2_map_row_major(*bit2, apply_print, NULL);
	// printf("Edges:\n");
	// Bit2_map_row_major(*edge_pixels, apply_print, NULL);
	
	Bit2_free(edge_pixels);
	free(edge_pixels);

	return;
}



int main(int argc, char *argv[]){
	assert(argc<=2);

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

	Pnmrdr_T *pbm;
	NEW(pbm);
	*pbm = Pnmrdr_new(stream);
	Pnmrdr_mapdata data = Pnmrdr_data(*pbm);
	assert(data.denominator==1);
	// printf("height: %d\nwidth: %d\ndenom: %d\n", data.height, data.width, data.denominator);
	
	Bit2_T *bit2;
	NEW(bit2);
	*bit2 = Bit2_new(data.height, data.width);
	// printf("height: %d\nwidth: %d\n", Bit2_height(*bit2), Bit2_width(*bit2));
	
	// test insertion
	// Bit2_map_row_major(*bit2, apply_print, NULL);

	fill_bit2(bit2, pbm);
	fclose(stream);
	unblack(bit2);

	// FILE *out = stdout;
	// pbmwrite(out, bit2);
	// fclose(out);

	Bit2_free(bit2);
	free(bit2);
	Pnmrdr_free(pbm);
	free(pbm);
	
	return 0;
}
