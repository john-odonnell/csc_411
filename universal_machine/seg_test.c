#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "seq.h"
#include "assert.h"
#include "mem.h"

#include "seg.h"

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
	
	// instantiate memory as a Seg struct
	Segs memory = seg_new();
	// assert it exists
	assert(memory);

	// map 0 seg
	uint32_t idx = seg_map(memory, 2);
	assert(idx == 0);

	// fill seg 0 words
	uint32_t test_1 =  ~0;
	uint32_t test_2 = 555;
	seg_fill(memory, test_1, 0, 0);
	seg_fill(memory, test_2, 0, 1);
	
	// check mapped segment
	uint32_t value_1 = seg_get(memory, 0, 0);
	uint32_t value_2 = seg_get(memory, 0, 1);
	printf("value 1: %x\n", value_1);
	printf("value 2: %x\n", value_2);

	// check unmap function and sequence
	assert(Seq_length(memory->unmapped) == 0);
	seg_unmap(memory, 0);
	assert(Seq_length(memory->unmapped) == 1);
	uint32_t idx_unmapped = (uint32_t)(uintptr_t)Seq_get(memory->unmapped, 0);
	printf("idx unmapped: %x\n", idx_unmapped);

	// try to map to 0 segment again
	idx = seg_map(memory, 3);
	assert(idx == 0);

	// free memory and pointer
	seg_free(memory);
	FREE(memory);

	return 0;
}
