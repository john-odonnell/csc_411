#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "mem.h"
#include "seq.h"

#include "execute.h"
#include "run_um.h"

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;

	uint32_t registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	uint32_t a = 0;
	uint32_t b = 1;
	uint32_t c = 2;

	registers[b] = 10;
	registers[c] = 3;

	uint32_t value = 69420;

	struct Op_Instruction instr = {13, a, b, c, value};

	load_value (&instr, registers);

	printf("Load Value result: %u\n", registers[0]);

	return 0;
}
