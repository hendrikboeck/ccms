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

#ifndef __CCMS__ARENAS__STATIC__H
#define __CCMS__ARENAS__STATIC__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef __CCMS__SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_defs.h"
#include "ccms/_macros.h"

typedef struct st_arena_t st_arena_t;

struct st_arena_t {
  uint8_t* writehead;
  size_t size;
};

__CCMS__INLINE
st_arena_t* st_arena__new(const size_t size) {
  st_arena_t* self = _M_cast(st_arena_t*, _M_alloc(sizeof(st_arena_t) + size));

  self->writehead = _M_cast(uint8_t*, self) + sizeof(st_arena_t);
  self->size = size;

  return self;
}

__CCMS__INLINE
void st_arena__free(st_arena_t* self) {
  _M_free(self);
}

__CCMS__INLINE
st_arena_t* st_arena__clone(const st_arena_t* self) {
  st_arena_t* other = st_arena__new(self->size);
  size_t wh_offset = _M_cast(size_t, self->writehead - _M_cast(uint8_t*, self));

  memcpy(other, self, self->size + sizeof(st_arena_t));
  other->writehead = _M_cast(uint8_t*, other) + wh_offset;

  return other;
}

__CCMS__INLINE
size_t st_arena__cap(const st_arena_t* self) {
  return (self->size + sizeof(st_arena_t)) -
         _M_cast(size_t, self->writehead - _M_cast(uint8_t*, self));
}

__CCMS__INLINE
void st_arena__reset(st_arena_t* self) {
  self->writehead = _M_cast(uint8_t*, self) + sizeof(st_arena_t);
}

__CCMS__INLINE
uint8_t* st_arena__alloc(st_arena_t* self, const size_t size) {
  if (st_arena__cap(self) < size) {
#ifndef __CCMS__SUPPRESS_WARNINGS
    fprintf(stderr,
            "warning: tried to allocate a chunk of memory of size %ld from an "
            "arena (static) with only %ld free memory, returned NULL\n",
            size, st_arena__cap(self));
#endif
    return NULL;
  }

  uint8_t* result = self->writehead;
  self->writehead += size;

  return result;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS_ARENAS_STATIC_H
