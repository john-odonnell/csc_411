#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

//Gets each value in the image, checks that it is in the valid range, and
//inserts it into the 2d array
void getVal(void* value, void* cl){

        Pnmrdr_T* data = (Pnmrdr_T*) cl;
        int n = (int) Pnmrdr_get(*data);
        //n is unsigned so the only possible invalid numbers are 0 or n>9
        //since any negative number will be translated to a positive in unsigned
        assert(n && n < 10);
        *((int*) value) = n;

}


/* TODO: Change to use A2 abstract type
 *       Row-Major should be usable for obtaining image data
 *       based on pnmrdr output
 */
void buildBoard(UArray2_T board, Pnmrdr_T data){

        UArray2_map_row_major(board,
                getVal,
                (void*) &data);
}

int main(int argc, char *argv[]) {
  int rotation = 0;
  A2Methods_T methods = array2_methods_plain; // default to UArray2 methods
  assert(methods);
  A2Methods_mapfun *map = methods->map_default; // default to best map
  assert(map);
#define SET_METHODS(METHODS, MAP, WHAT) do { \
      methods = (METHODS); \
      assert(methods); \
      map = methods->MAP; \
      if (!map) { \
        fprintf(stderr, "%s does not support " WHAT "mapping\n", argv[0]); \
        exit(1); \
      } \
    } while(0)

  int i;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-row-major")) {
      SET_METHODS(array2_methods_plain, map_row_major, "row-major");
    } else if (!strcmp(argv[i], "-col-major")) {
      SET_METHODS(array2_methods_plain, map_col_major, "column-major");
    } else if (!strcmp(argv[i], "-block-major")) {
      SET_METHODS(array2_methods_blocked, map_block_major, "block-major");
    } else if (!strcmp(argv[i], "-rotate")) {
      assert(i + 1 < argc);
      char *endptr;
      rotation = strtol(argv[++i], &endptr, 10);
      assert(*endptr == '\0'); // parsed all correctly
      assert(rotation == 0   || rotation == 90
          || rotation == 180 || rotation == 270);
    } else if (*argv[i] == '-') {
      fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
      exit(1);
    } else if (argc - i > 2) {
      fprintf(stderr, "Usage: %s [-rotate <angle>] "
              "[-{row,col,block}-major] [filename]\n", argv[0]);
      exit(1);
    } else {
      break;
    }
  }
   assert(0); // the rest of this function is not yet implemented
}


