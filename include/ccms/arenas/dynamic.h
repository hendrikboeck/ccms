#ifndef __CCMS_ARENAS_DYNAMIC_H
#define __CCMS_ARENAS_DYNAMIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_macros.h"

typedef struct _DynamicArenaBlock _DynamicArenaBlock;
typedef struct DynamicArena DynamicArena;

struct _DynamicArenaBlock {
  _DynamicArenaBlock *next;
  size_t size;
};

extern inline _DynamicArenaBlock *
_dynamic_arena_block__new(const size_t size, _DynamicArenaBlock *next) {
  _DynamicArenaBlock *self = _M_cast(
      _DynamicArenaBlock *, _M_alloc(sizeof(_DynamicArenaBlock) + size));

  self->next = next;
  self->size = size;

  return self;
}

extern inline void _dynamic_arena_block__free(_DynamicArenaBlock *self) {
  _M_free(self);
}

extern inline _DynamicArenaBlock *
_dynamic_arena_block__clone(const _DynamicArenaBlock *self) {
  _DynamicArenaBlock *other = _M_cast(
      _DynamicArenaBlock *, _M_alloc(sizeof(_DynamicArenaBlock) + self->size));
  memcpy(other, self, sizeof(_DynamicArenaBlock) + self->size);

  return other;
}

struct DynamicArena {
  _DynamicArenaBlock *head, *tail;
};

extern inline DynamicArena *dynamic_arena__new() {
  DynamicArena *self = _M_new(DynamicArena);

  self->head = self->tail = NULL;

  return self;
}

extern inline void dynamic_arena__reset(DynamicArena *self) {
  for (_DynamicArenaBlock *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _dynamic_arena_block__free(itr);
  }

  self->head = self->tail = NULL;
}

extern inline void dynamic_arena__free(DynamicArena *self) {
  dynamic_arena__reset(self);
  _M_free(self);
}

extern inline uint8_t *dynamic_arena__alloc(DynamicArena *self,
                                            const size_t size) {
  _DynamicArenaBlock *new_block = _dynamic_arena_block__new(size, NULL);

  if (self->tail != NULL)
    self->tail->next = new_block;
  else
    self->head = new_block;
  self->tail = new_block;

  return _M_cast(uint8_t *, new_block) + sizeof(_DynamicArenaBlock);
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS_ARENAS_DYNAMIC_H
