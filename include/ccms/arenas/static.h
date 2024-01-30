#ifndef __CCMS__ARENAS__STATIC__H
#define __CCMS__ARENAS__STATIC__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef __CCMS_SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_macros.h"

typedef struct StaticArena StaticArena;

struct StaticArena {
  uint8_t *writehead;
  size_t size;
};

extern inline StaticArena *static_arena__new(const size_t size) {
  StaticArena *self =
      _M_cast(StaticArena *, _M_alloc(sizeof(StaticArena) + size));

  self->writehead = _M_cast(uint8_t *, self) + sizeof(StaticArena);
  self->size = size;

  return self;
}

extern inline void static_arena__free(StaticArena *self) { _M_free(self); }

extern inline StaticArena *static_arena__clone(const StaticArena *self) {
  StaticArena *other = static_arena__new(self->size);
  size_t wh_offset =
      _M_cast(size_t, self->writehead - _M_cast(uint8_t *, self));

  memcpy(other, self, self->size + sizeof(StaticArena));
  other->writehead = _M_cast(uint8_t *, other) + wh_offset;

  return other;
}

extern inline size_t static_arena__cap(const StaticArena *self) {
  return (self->size + sizeof(StaticArena)) -
         _M_cast(size_t, self->writehead - _M_cast(uint8_t *, self));
}

extern inline void static_arena__reset(StaticArena *self) {
  self->writehead = _M_cast(uint8_t *, self) + sizeof(StaticArena);
}

extern inline uint8_t *static_arena__alloc(StaticArena *self,
                                           const size_t size) {
  if (static_arena__cap(self) < size) {
#ifndef __CCMS_SUPPRESS_WARNINGS
    fprintf(stderr,
            "warning: tried to allocate a chunk of memory of size %ld from an "
            "arena (static) with only %ld free memory, returned NULL\n",
            size, static_arena__cap(self));
#endif
    return NULL;
  }

  uint8_t *result = self->writehead;
  self->writehead += size;

  return result;
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS_ARENAS_STATIC_H
