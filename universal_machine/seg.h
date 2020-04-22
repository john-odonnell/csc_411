#include <stdint.h>
#include "seq.h"

struct Segs {
	Seq_T segments;
	Seq_T mapped;
	Seq_T unmapped;
} *Segs;

extern Segs     seg_new   ();
extern void     seg_free  (Segs segments);
extern uint32_t seg_map   (Segs segments, uint32_t size);
extern void     seg_unmap (Segs segments, uint32_t id);
extern void	seg_fill  (Segs segments, uint32_t values, uint32_t id, uint32_t offset);
extern void     seg_get   (Segs segments, uint32_t id, uint32_t offset);
