#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "assert.h"
#include "seq.h"

#include "execute.h"
#include "run_um.h"

void load_zero(FILE *stream, Segs memory) {	
	// initialize zero segment
	Seq_T zero_seg = Seq_new(25);
	
	// until the loop reaches the EOF
	while(true){
		// initialize current word to 0
		uint32_t word = 0;
		// scan for unsigned integer or break loop
		int u = fscanf(stream, "%u", &word);
		if (u == 0) {
			break;
		}
		
		// add the word to the zero sequence
		Seq_addhi(zero_seg, (void*)(uintptr_t)word);
	}
	
	// for the number of words in the zero segment
	// transfer each 32 bit word from the zero_seg sequence into memory
	int words = Seq_length(zero_seg);
	seg_map(memory, words);
	for (int i = 0; i < words; i++) {
		seg_fill(memory, (uint32_t)(uintptr_t)Seq_get(zero_seg, i), 0, i);
	}

	Seq_free(&zero_seg);

	return;
}

Op_Instruction get_opcode(uint32_t word) {
	// isolate opcode
	uint32_t opcode;
	uint32_t opcode_isolator = 4026531840;
	opcode = (word & opcode_isolator);
   	opcode = opcode >> 28;
	
	// initialize register id's and value
	// along with isolating ints for all
	uint32_t     a = 0;
	uint32_t     b = 0;
	uint32_t     c = 0;
       	uint32_t value = 0;
	uint32_t a_isolator;
	uint32_t b_isolator =  56;
       	uint32_t c_isolator =   7;
	if (opcode != 13) {
		// isolate a, b and c
		a_isolator = 448;
		a = (word & a_isolator); a = a >> 6;
		b = (word & b_isolator); b = b >> 3;
		c = (word & c_isolator);
	} else {
		// isolate a and value
		a_isolator = 234881024;
		a = (word & a_isolator); a = a >> 25;
		value = (word); value = value << 7; value = value >> 7;
	}
	
	// insert opcode, a, b, c and value into a struct and return
	Op_Instruction word_info;
	NEW(word_info);
	word_info->opcode = opcode;
	word_info->a = a;
	word_info->b = b;
	word_info->c = c;
	word_info->value = value;

	return word_info;
}

void run(FILE *stream) {
	// create new address space and
	// load the program into the zero seg
	Segs memory = seg_new();
	load_zero(stream, memory);
	
	// initialize registers and program counter
	uint32_t registers[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t pro_counter = 0;
	
	// until the loop is told to break...
	while(true) {
		// pull word from the zero seg and break into components
		uint32_t word = seg_get(memory, 0, pro_counter);
		Op_Instruction this_instr = get_opcode(word);
		
		// pass the instruction to a function depending on opcode
		int opcode = (int)(this_instr->opcode);
		switch (opcode) {
			case 0:
				cond_move(this_instr, registers);
				break;
			case 1:
				seg_load(this_instr, memory, registers);
				break;
			case 2:
				seg_store(this_instr, memory, registers);
				break;
			case 3:
				add(this_instr, registers);
				break;
			case 4:
				mult(this_instr, registers);
				break;
			case 5:
				divy(this_instr, registers);
				break;
			case 6:
				nand(this_instr, registers);
				break;
			case 7:
				halt(this_instr, memory, registers);
				break;
			case 8:
				map_seg(this_instr, memory, registers);
				break;
			case 9:
				unmap_seg(this_instr, memory, registers);
				break;
			case 10:
				output(this_instr, stdout, registers);
				break;
			case 11:
				input(this_instr, stdin, registers);
				break;
			case 12:
				load_program(this_instr, memory, registers);
				break;
			case 13:
				load_value(this_instr, registers);
				break;
			default:
				break;
		};
		
		// if the program received the halt instruction,
		// break the execution loop
		if (opcode == 7) {
			break;
		}
		
		// increment program counter
		pro_counter++;
	}

	return;
}
