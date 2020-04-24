#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "seq.h"

#include "run_um.h"
#include "seg.h"

#ifndef EXECUTE_INCLUDED
#define EXECUTE_INCLUDED

#define OP Op_Instruction

// if the value in register c is not 0, then the value in
// register a is stored in register b
extern void cond_move 	 (OP instr, uint32_t *reg);

// the value in the reg[c] word of the reg[b] sequence is loaded into reg[a]
extern void seg_load 	 (OP instr, Segs memory, uint32_t *reg);

// the value in reg[c] is stored in the reg[b] word of the reg[a] segment
extern void seg_store 	 (OP instr, Segs memory, uint32_t *reg);

// the sum of the values in registers b and c is stored in register a
extern void add 	 (OP instr, uint32_t *reg);

// the product of the values in registers b and c is stored in register a
extern void mult 	 (OP instr, uint32_t *reg);

// the division of the value in registers b by c is stored in register a
extern void divy 	 (OP instr, uint32_t *reg);

// bitwise NAND operation :: ~(reg[b] ^ reg[c])
extern void nand 	 (OP instr, uint32_t *reg);

// halt command, ends computation and frees memory from the machine
extern void halt 	 (OP instr, Segs memory, uint32_t *reg);

// a new segment is created with reg[c] words
// stores the segment ID of the newly mapped segment in reg[b]
extern void map_seg 	 (OP instr, Segs memory, uint32_t *reg);

// unmapped the segment of ID reg[c]
// the segment is made available for future mappings
extern void unmap_seg 	 (OP instr, Segs memory, uint32_t *reg);

// the value in reg[c] is output to the given file stream
extern void output 	 (OP instr, FILE *stream, uint32_t *reg);

// the machine takes input from the file stream
// the input is loaded into reg[c] :: end of input loads reg[c] with ~0
extern void input 	 (OP instr, FILE *stream, uint32_t *reg);

// the reg[b] segment is duplicated and replaces the zero segment
// the program counter is set to the reg[c] word
// if the value in reg[b] is 0, then this results in just a program counter jump
extern void load_program (OP instr, Segs memory, uint32_t *reg, uint32_t *pro_counter);

// loads the value in the least significant 25 bits of the instruction word
// into reg[a]
extern void load_value   (OP instr, uint32_t *reg);

#undef OP
#endif
