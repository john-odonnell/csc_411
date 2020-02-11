#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T

typedef struct T *T;
struct T { // Bit2_T struct definition
	int height;		// height of 2d bit array
	int width;		// width of 2d bit array
	char *array;
};

extern T Bit2_new (int height, int width);
// returns a new Bit2_T struct allocated on the heap based on the given height and width
extern void Bit2_free (T *bit2);
// frees the memory allocated for the given Bit2_T along with its contained array

extern int Bit2_height (T bit2);
// returns the height of a given Bit2_T
extern int Bit2_width (T bit2);
// returns the width of a given Bit2_T

extern int Bit2_get(T bit2, int height, int width);
// returns the bit value of the (i, j) position in the given Bit2_T array
extern int Bit2_put(T bit2, int height, int width, int bit);
// changes the value of the bit at the (i, j) position in the given Bit2_T array to bit

extern void Bit2_map_row_major(T bit2, void apply(int n, int bit, void *cl), void *cl);
// calls the apply() function on each bit in the given Bit2_T array in row major order
extern void Bit2_map_col_major(T bit2, void apply(int n, int bit, void *cl), void *cl);
// calls the apply() function on each bit in the given Bit2_T array in column major order

#undef T
#endif
