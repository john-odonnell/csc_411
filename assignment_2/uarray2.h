#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#define T UArray2_T

typedef struct T *T;
struct T {
	int height;
	int width;
	int size;
	int *array;
};

extern T UArray2_new (int height, int width, int size);
extern void UArray2_free (T *uarray2);

extern int UArray2_height (T uarray2);
extern int UArray2_width (T uarray2);
extern int UArray2_size (T uarray2);

extern void *UArray2_at (T uarray2, int height, int width);

extern void UArray2_map_row_major (T uarray2, void apply(void *n, int bit, void *cl), void *cl);
// extern void UArray2_map_col_major (T uarray2, void apply(int n, int bit, void *cl), void *cl);

#undef T
#endif
