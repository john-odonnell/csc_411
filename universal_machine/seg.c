#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "seq.h"
#include "mem.h"
#include "assert.h"

#include "seg.h"
#include "bitpack.h"

// instantiates a new segs struct
// segs includes the segments address space
// along with mapped and unmapped addresses
Segs seg_new() {
	Segs new_seg;
	NEW(new_seg);

	// instatiate members and assign
	new_seg->segments = Seq_new(0);
	new_seg->unmapped = Seq_new(0);

	// assert instatiation
	assert(new_seg);
	assert(new_seg->segments);
	assert(new_seg->unmapped);

	new_seg->highest = -1;

	return new_seg;
}

// frees segs struct and members
void seg_free(Segs segs) {
	// assert segs and members exists
	assert(segs);
	assert(segs->segments);
	assert(segs->unmapped);

	// free memory for each segment sequence
	int len = Seq_length(segs->segments);
	for (int i = 0; i < len; i++) {
		Seq_T this_seg = Seq_remlo(segs->segments);
		if (this_seg != NULL) {
			Seq_free(&this_seg);
		}
	}

	// free member sequences
	Seq_free((Seq_T*)&segs->segments);
	Seq_free((Seq_T*)&segs->unmapped);

	return;
}

// create a new segment with the
// number of words equal to size
uint32_t seg_map(Segs segments, uint32_t size) {
	// 32 0 bits
	uint32_t x = 0;
	
	// new sequence of uint32_t's
	Seq_T this_seg = Seq_new(size);
	assert(this_seg);

	for (int i = 0; i < (int)size; i++) {
		Seq_addlo(this_seg, (void*)(uintptr_t)x);
	}
	
	// index variable
	uint32_t idx;
	// if there are no unmapped identifiers :: increment highest seg ID
	//                                      :: use Seq_addhi, preserves 0 seg at 0
	// 			      otherwise :: the seg ID is the lowest available umapped ID
	// 			      		:: use Seq_put to insert directly
	if (Seq_length(segments->unmapped) == 0) {
		segments->highest += 1;
		idx = segments->highest;
			
		Seq_addhi(segments->segments, (void*)this_seg);
	} else {
		idx = (uint32_t)(uintptr_t)Seq_remlo(segments->unmapped);

		Seq_T current_seg = Seq_get(segments->segments, (int)idx);
		Seq_free(&current_seg);

		Seq_put(segments->segments, (int)idx, (void*)this_seg);
	}

	return idx;
}

// unmap a segment of the given ID
void seg_unmap(Segs segments, uint32_t id) {
	// get pointer to segment sequence and free
	// Seq_T this_seg = Seq_get(segments->segments, (int)id);
	// Seq_free(&this_seg);
	
	// add the unmapped id to the sequence of unmapped ids
	Seq_addlo(segments->unmapped, (void*)(uintptr_t)id);

	return;
}

// fills a segment with a value at an offset
void seg_fill(Segs segments, uint32_t value, uint32_t id, uint32_t offset) {
	// get segment sequence
	Seq_T this_seg = Seq_get(segments->segments, (int)id);

	// get segment word
	Seq_put(this_seg, (int)offset, (void*)(uintptr_t)value);

	return;
}

uint32_t seg_get(Segs segments, uint32_t id, uint32_t offset) {
	// get segment sequence
	Seq_T this_seg = Seq_get(segments->segments, (int)id);

	// return segment word
	return (uint32_t)(uintptr_t)Seq_get(this_seg, (int)offset);
}
