#ifndef __CCMS__SIZED_MEMORY__H
#define __CCMS__SIZED_MEMORY__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_macros.h"
#include "ccms/box.h"

typedef struct SizedMemory SizedMemory;

struct SizedMemory {
  uint8_t *ptr;
  size_t size;
};

static inline SizedMemory *sized_memory__new(const size_t size) {
  SizedMemory *self =
      _M_cast(SizedMemory *, _M_alloc(sizeof(SizedMemory) + size));

  self->ptr = _M_cast(uint8_t *, self) + sizeof(SizedMemory);
  self->size = size;

  return self;
}

static inline void sized_memory__free(SizedMemory *self) { _M_free(self); }

static inline SizedMemory *sized_memory__clone(const SizedMemory *self) {
  SizedMemory *other = sized_memory__new(self->size);

  memcpy(other->ptr, self->ptr, self->size);

  return other;
}

static inline Box sized_memory__as_box(const SizedMemory *self) {
  return box__ctor(self->ptr, self->size);
}

static inline SizedMemory *sized_memory__from_box(const Box box) {
  SizedMemory *self = sized_memory__new(box.size);

  memcpy(self->ptr, box.ptr, box.size);

  return self;
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS__SIZED_MEMORY__H
