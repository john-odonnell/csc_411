#include <stdint.h>
#include "seq.h"

#ifndef SEG_INCLUDED
#define SEG_INCLUDED

// Segs struct :: universal machine memory
// contains a Hanson sequence of segments,
// a Hanson sequence of unmapped segment IDs,
// and a highest mapped segment value
typedef struct Segs {
	Seq_T segments;
	Seq_T unmapped;
	int highest;
} *Segs;

// creates and returns a new Segs address space
extern Segs     seg_new   ();

// frees the given Segs address space
extern void     seg_free  (Segs segments);

// a new segment is created with size words
// the new segment is either located at the most recently unmapped segment
// or at the end of the segment sequence
extern uint32_t seg_map   (Segs segments, uint32_t size);

// unmaps the segment at the given seg ID
extern void     seg_unmap (Segs segments, uint32_t id);

// fills the offset word of the ID segment with value
extern void	seg_fill  (Segs segments, uint32_t values, uint32_t id, uint32_t offset);

// returns the offset word of the ID segment
extern uint32_t seg_get   (Segs segments, uint32_t id, uint32_t offset);

#endif
