/******************************************************************************/
/* CCMS - Collection of C Memory Structures, a lightweight header-only C      */
/* library.                                                                   */
/* Copyright (C) 2024, Hendrik Boeck <hendrikboeck.dev@protonmail.com>        */
/*                                                                            */
/* This program is free software: you can redistribute it and/or modify  it   */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation, either version 3 of the License, or (at your option)  */
/* any later version.                                                         */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but        */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY */
/* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   */
/* for more details.                                                          */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program.  If not, see <https://www.gnu.org/licenses/>.           */
/******************************************************************************/

#ifndef __CCMS__ARENAS__DYNAMIC__H
#define __CCMS__ARENAS__DYNAMIC__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_defs.h"
#include "ccms/_macros.h"

typedef struct _dyn_arena_block_t _dyn_arena_block_t;

struct _dyn_arena_block_t {
  _dyn_arena_block_t* next;
  size_t size;
};

__CCMS__INLINE
_dyn_arena_block_t* _dyn_arena_block__new(const size_t size,
                                          _dyn_arena_block_t* next) {
  _dyn_arena_block_t* self =
      _M_cast(_dyn_arena_block_t*, _M_alloc(sizeof(_dyn_arena_block_t) + size));

  self->next = next;
  self->size = size;

  return self;
}

__CCMS__INLINE
void _dyn_arena_block__free(_dyn_arena_block_t* self) {
  _M_free(self);
}

__CCMS__INLINE
_dyn_arena_block_t* _dyn_arena_block__clone(const _dyn_arena_block_t* self) {
  _dyn_arena_block_t* other = _M_cast(
      _dyn_arena_block_t*, _M_alloc(sizeof(_dyn_arena_block_t) + self->size));
  memcpy(other, self, sizeof(_dyn_arena_block_t) + self->size);

  return other;
}

typedef struct dyn_arena_t dyn_arena_t;

struct dyn_arena_t {
  _dyn_arena_block_t *head, *tail;
};

__CCMS__INLINE
dyn_arena_t* dyn_arena__new() {
  dyn_arena_t* self = _M_new(dyn_arena_t);

  self->head = self->tail = NULL;

  return self;
}

__CCMS__INLINE
void dyn_arena__reset(dyn_arena_t* self) {
  for (_dyn_arena_block_t *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _dyn_arena_block__free(itr);
  }

  self->head = self->tail = NULL;
}

__CCMS__INLINE
void dyn_arena__free(dyn_arena_t* self) {
  dyn_arena__reset(self);
  _M_free(self);
}

__CCMS__INLINE
uint8_t* dyn_arena__alloc(dyn_arena_t* self, const size_t size) {
  _dyn_arena_block_t* new_block = _dyn_arena_block__new(size, NULL);

  if (self->tail != NULL)
    self->tail->next = new_block;
  else
    self->head = new_block;
  self->tail = new_block;

  return _M_cast(uint8_t*, new_block) + sizeof(_dyn_arena_block_t);
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__ARENAS__DYNAMIC__H
