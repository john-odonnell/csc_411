#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "seq.h"

#include "run_um.h"
#include "seg.h"

#ifndef EXECUTE_INCLUDED
#define EXECUTE_INCLUDED

#define OP Op_Instruction

extern void cond_move 	 (OP instr, uint32_t *reg);
extern void seg_load 	 (OP instr, Segs memory, uint32_t *reg);
extern void seg_store 	 (OP instr, Segs memory, uint32_t *reg);
extern void add 	 (OP instr, uint32_t *reg);
extern void mult 	 (OP instr, uint32_t *reg);
extern void divy 	 (OP instr, uint32_t *reg);
extern void nand 	 (OP instr, uint32_t *reg);
extern void halt 	 (OP instr, Segs memory, uint32_t *reg);
extern void map_seg 	 (OP instr, Segs memory, uint32_t *reg);
extern void unmap_seg 	 (OP instr, Segs memory, uint32_t *reg);
extern void output 	 (OP instr, FILE *stream, uint32_t *reg);
extern void input 	 (OP instr, FILE *stream, uint32_t *reg);
extern void load_program (OP instr, Segs memory, uint32_t *reg);
extern void load_value   (OP instr, uint32_t *reg);

#undef OP
#endif
