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

#ifndef __CCMS__ARENAS__PAGED__H
#define __CCMS__ARENAS__PAGED__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#ifndef __CCMS__SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_defs.h"
#include "ccms/_macros.h"

typedef struct _pg_arena_page_t _pg_arena_page_t;

struct _pg_arena_page_t {
  _pg_arena_page_t* next;
  size_t pos;
};

__CCMS__INLINE
_pg_arena_page_t* _pg_arena_page__new(const size_t size,
                                      _pg_arena_page_t* next) {
  _pg_arena_page_t* self =
      _M_cast(_pg_arena_page_t*, _M_alloc(sizeof(_pg_arena_page_t) + size));

  self->pos = 0;
  self->next = next;

  return self;
}

__CCMS__INLINE
void _pg_arena_page__free(_pg_arena_page_t* self) {
  _M_free(self);
}

__CCMS__INLINE
void _pg_arena_page__reset(_pg_arena_page_t* self) {
  self->pos = 0;
}

typedef struct pg_arena_t pg_arena_t;

struct pg_arena_t {
  _pg_arena_page_t *head, *tail;
  size_t page_size;
};

__CCMS__INLINE
pg_arena_t* pg_arena__new(const size_t page_size) {
  pg_arena_t* self = _M_new(pg_arena_t);

  self->page_size = page_size;
  self->head = self->tail = _pg_arena_page__new(page_size, NULL);

  return self;
}

__CCMS__INLINE
void pg_arena__free(pg_arena_t* self) {
  for (_pg_arena_page_t *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _pg_arena_page__free(itr);
  }
  _M_free(self);
}

__CCMS__INLINE
void pg_arena__reset(pg_arena_t* self) {
  for (_pg_arena_page_t* itr = self->head; itr != NULL; itr = itr->next)
    _pg_arena_page__reset(itr);

  self->tail = self->head;
}

__CCMS__INLINE
void pg_arena__hard_reset(pg_arena_t* self) {
  if (self->head->next != NULL)
    for (_pg_arena_page_t *itr = self->head->next, *tmp; itr != NULL;
         itr = tmp) {
      tmp = itr->next;
      _pg_arena_page__free(itr);
    }

  _pg_arena_page__reset(self->head);
  self->head->next = NULL;
  self->tail = self->head;
}

__CCMS__INLINE
uint8_t* pg_arena__alloc(pg_arena_t* self, const size_t size) {
  // If the requested size is larger than the page size of the pg_arena_t
  if (size > self->page_size) {
#ifndef __CCMS__SUPPRESS_WARNINGS
    // Print a warning message
    fprintf(stderr,
            "warning: tried allocating a chunk of size %ld from an arena (page "
            "allocated) with page size %ld, returned NULL\n",
            size, self->page_size);
#endif
    // Return NULL as we cannot allocate a chunk larger than the page size
    return NULL;
  }

  // If the remaining space in the current page is less than the requested size
  if (self->page_size - self->tail->pos < size) {
    if (self->tail->next == NULL)
      // Allocate a new page and set it as the next page
      self->tail->next = _pg_arena_page__new(self->page_size, NULL);

    // Move tail to the next page
    self->tail = self->tail->next;
  }

  // Calculate the address of the new chunk by adding the size of the
  // _pg_arena_page_t struct and the current position to the address of the
  // current page
  uint8_t* result = _M_cast(uint8_t*, self->tail) + sizeof(_pg_arena_page_t) +
                    self->tail->pos;
  // Update the position in the current page
  self->tail->pos += size;

  // Return the address of the new chunk
  return result;
}

__CCMS__INLINE
float pg_arena__avg_util(const pg_arena_t* self) {
  float sum = 0.f;
  size_t len = 0;

  for (_pg_arena_page_t* itr = self->head; itr != NULL; itr = itr->next, len++)
    sum += _M_cast(float, itr->pos) / self->page_size;

  return sum / len;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__ARENAS__PAGED__H
