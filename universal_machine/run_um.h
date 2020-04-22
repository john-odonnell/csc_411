#include <stdio.h>
#include <stdint.h>

#include "seg.h"

#ifndef RUN_UM_INCLUDED
#define RUN_UM_INCLUDED

typedef struct Op_Instruction {
	uint32_t opcode;
	uint32_t a, b, c;
	uint32_t value;
} *Op_Instruction;

extern void 	      load_zero  (FILE *stream, Segs memory);
extern Op_Instruction get_opcode (uint32_t word);
extern void	      run        (FILE *stream);

#endif
