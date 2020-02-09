#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T

typedef struct T *T;
struct T { // Bit2_T struct definition
	int height;
	int width;
	unsigned char *bytes;
	unsigned long *words;
};

extern T Bit2_new (int height, int width);
extern void Bit2_free (T *bit2);

extern int Bit2_height (T bit2);
extern int Bit2_width (T bit2);

extern int Bit2_get(T bit2, int height, int width);
extern int Bit2_put(T bit2, int height, int width, int bit);

extern void Bit2_map_rpw_major(T bit2, void apply(int n, int bit, void *cl), void *cl);
extern void Bit2_map_col_major(T bit2, void apply(int n, int bit, void *cl), void *cl);

#undef T
#endif
