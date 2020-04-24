#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "mem.h"
#include "seq.h"

#include "run_um.h"
#include "seg.h"
#include "execute.h"

#define OP Op_Instruction

// if the value in reg c is not 0
// load the value in reg b into reg a
void cond_move(OP instr, uint32_t *reg) {
	if ((int)reg[instr->c] != 0) {
		reg[instr->a] = reg[instr->b];
	}
	return;
}

// load a value into reg a from a memory segment word
void seg_load(OP instr, Segs memory, uint32_t *reg) {
	reg[instr->a] = seg_get(memory, reg[instr->b], reg[instr->c]);
	return;
}

// store the value in reg c into the b word of the a segment
void seg_store(OP instr, Segs memory, uint32_t *reg) {
	seg_fill(memory, reg[instr->c], reg[instr->a], reg[instr->b]);
	return;
}

// add the value in reg b and the value in reg c
// store value in reg a
void add(OP instr, uint32_t *reg) {
	reg[instr->a] = (reg[instr->b] + reg[instr->c]) % (4294967296);
	return;
}

// multiply the value in reg b with the value in reg c
// store value in reg a
void mult(OP instr, uint32_t *reg) {
	reg[instr->a] = (reg[instr->b] * reg[instr->c]) % (4294967296);
	return;
}

// divide the value in reg b by the value in reg c
// store in reg a
void divy(OP instr, uint32_t *reg) {
	reg[instr->a] = (reg[instr->b] / reg[instr->c]);
	return;
}

// logical NAND the bits in reg b and reg c
// store result in reg a
void nand(OP instr, uint32_t *reg) {
	reg[instr->a] = ~(reg[instr->b] & reg[instr->c]);
	return;
}

// halt operation of the um
// free memory occupied by memory and registers
void halt(OP instr, Segs memory, uint32_t *reg) {
	(void)instr; (void)reg;
	seg_free(memory);
	FREE(memory);
	FREE(instr);
	return;
}

// maps a segment with number of words equal to the value in reg c
// store the seg id in reg b
void map_seg(OP instr, Segs memory, uint32_t *reg) {
	reg[instr->b] = seg_map(memory, reg[instr->c]);
	return;
}

// unmap a segment of seg id equal to the value in reg c
void unmap_seg(OP instr, Segs memory, uint32_t *reg) {
	seg_unmap(memory, reg[instr->c]);
	return;
}

// output the value in reg c to a file stream
void output(OP instr, FILE *stream, uint32_t *reg) {
	fprintf(stream, "%c", (char)reg[instr->c]);
	return;
}

// input the value given as input into reg c
void input(OP instr, FILE *stream, uint32_t *reg) {
	reg[instr->c] = (uint32_t)fgetc(stream);
	return;
}

// loads a segment of id equal to the value in reg b into the zero seg
void load_program(OP instr, Segs memory, uint32_t *reg, uint32_t *pro_counter) {
	int b = (int)reg[instr->b];
	
	if(b == 0){
		*pro_counter = reg[instr->c];
		return;
	}

	// unmap zero seg
	seg_unmap(memory, 0);
	
	// pull desired segment from memory
	Seq_T this_seg = Seq_get(memory->segments, b);
	int words = Seq_length(this_seg);
	
	// map a new zero seg
	seg_map(memory, words);

	// copy each word from the original segment to the new zero seg
	for (int i = 0; i < words; i++) {
		seg_fill(memory, seg_get(memory, b, i), 0, i);
	}

	*pro_counter = reg[instr->c];

	return;
}

// loads the value in the command word into reg a
void load_value(OP instr, uint32_t *reg) {
	reg[instr->a] = instr->value;
	return;
}

#undef OP
