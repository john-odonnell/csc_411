#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include "assert.h"
#include "compress.h"


extern void remove_right(Pnm_ppm *ppm);
extern void remove_bottom(Pnm_ppm *ppm);
extern void trim_image(Pnm_ppm *ppm);

