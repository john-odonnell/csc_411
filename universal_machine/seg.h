#include <stdint.h>
#include "seq.h"

#ifndef SEG_INCLUDED
#define SEG_INCLUDED

typedef struct Segs {
	Seq_T segments;
	Seq_T unmapped;
	int highest;
} *Segs;

extern Segs     seg_new   ();
extern void     seg_free  (Segs segments);
extern uint32_t seg_map   (Segs segments, uint32_t size);
extern void     seg_unmap (Segs segments, uint32_t id);
extern void	seg_fill  (Segs segments, uint32_t values, uint32_t id, uint32_t offset);
extern uint32_t seg_get   (Segs segments, uint32_t id, uint32_t offset);

#endif
