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

#ifndef __CCMS_ARENAS_DYNAMIC_H
#define __CCMS_ARENAS_DYNAMIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_defs.h"
#include "ccms/_macros.h"

/**
 * @typedef _DynamicArenaBlock
 * @brief Typedef for struct _DynamicArenaBlock
 *
 * This typedef provides a shorthand for the struct _DynamicArenaBlock.
 */
typedef struct _DynamicArenaBlock _DynamicArenaBlock;

/**
 * @typedef DynamicArena
 * @brief Typedef for struct DynamicArena
 *
 * This typedef provides a shorthand for the struct DynamicArena.
 */
typedef struct DynamicArena DynamicArena;

/**
 * @struct _DynamicArenaBlock
 * @brief A block of memory in a dynamic arena.
 *
 * This struct represents a block of memory in a dynamic arena. Each block has a
 * pointer to the next block and a size.
 *
 * @var _DynamicArenaBlock::next
 * Pointer to the next block in the dynamic arena.
 *
 * @var _DynamicArenaBlock::size
 * The size of the block in bytes.
 */
struct _DynamicArenaBlock {
  _DynamicArenaBlock* next;
  size_t size;
};

/**
 * @brief Constructs a new _DynamicArenaBlock object.
 *
 * This function constructs a new _DynamicArenaBlock object with the given size
 * and next block.
 *
 * @param size The size of the block in bytes.
 * @param next Pointer to the next block in the dynamic arena.
 *
 * @return A new _DynamicArenaBlock object.
 */
__CCMS__INLINE
_DynamicArenaBlock* _dynamic_arena_block__new(const size_t size,
                                              _DynamicArenaBlock* next) {
  _DynamicArenaBlock* self =
      _M_cast(_DynamicArenaBlock*, _M_alloc(sizeof(_DynamicArenaBlock) + size));

  self->next = next;
  self->size = size;

  return self;
}

/**
 * @brief Frees a _DynamicArenaBlock object.
 *
 * This function frees a _DynamicArenaBlock object.
 *
 * @param self The _DynamicArenaBlock object to free.
 */
__CCMS__INLINE
void _dynamic_arena_block__free(_DynamicArenaBlock* self) {
  _M_free(self);
}

/**
 * @brief Clones a _DynamicArenaBlock object.
 *
 * This function clones a _DynamicArenaBlock object, creating a new
 * _DynamicArenaBlock with the same size and next block as the original.
 *
 * @param self The _DynamicArenaBlock object to clone.
 *
 * @return A new _DynamicArenaBlock object with the same size and next block as
 * the original.
 */
__CCMS__INLINE
_DynamicArenaBlock* _dynamic_arena_block__clone(
    const _DynamicArenaBlock* self) {
  _DynamicArenaBlock* other = _M_cast(
      _DynamicArenaBlock*, _M_alloc(sizeof(_DynamicArenaBlock) + self->size));
  memcpy(other, self, sizeof(_DynamicArenaBlock) + self->size);

  return other;
}

/**
 * @struct DynamicArena
 * @brief A dynamic memory arena.
 *
 * This struct represents a dynamic memory arena. It contains pointers to the
 * head and tail blocks of the arena.
 *
 * @var DynamicArena::head
 * Pointer to the first block in the dynamic arena.
 *
 * @var DynamicArena::tail
 * Pointer to the last block in the dynamic arena.
 */
struct DynamicArena {
  _DynamicArenaBlock *head, *tail;
};

/**
 * @brief Constructs a new DynamicArena object.
 *
 * This function constructs a new DynamicArena object.
 *
 * @return A new DynamicArena object.
 */
__CCMS__INLINE
DynamicArena* dynamic_arena__new() {
  DynamicArena* self = _M_new(DynamicArena);

  self->head = self->tail = NULL;

  return self;
}

/**
 * @brief Resets a DynamicArena object.
 *
 * This function resets a DynamicArena object, freeing all of its blocks.
 *
 * @param self The DynamicArena object to reset.
 */
__CCMS__INLINE
void dynamic_arena__reset(DynamicArena* self) {
  for (_DynamicArenaBlock *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _dynamic_arena_block__free(itr);
  }

  self->head = self->tail = NULL;
}

/**
 * @brief Frees a DynamicArena object.
 *
 * This function frees a DynamicArena object, freeing all of its blocks.
 *
 * @param self The DynamicArena object to free.
 */
__CCMS__INLINE
void dynamic_arena__free(DynamicArena* self) {
  dynamic_arena__reset(self);
  _M_free(self);
}

/**
 * @brief Allocates memory in a DynamicArena object.
 *
 * This function allocates memory in a DynamicArena object.
 *
 * @param self The DynamicArena object to allocate memory in.
 * @param size The size of the memory to allocate in bytes.
 *
 * @return A pointer to the allocated memory.
 */
__CCMS__INLINE
uint8_t* dynamic_arena__alloc(DynamicArena* self, const size_t size) {
  _DynamicArenaBlock* new_block = _dynamic_arena_block__new(size, NULL);

  if (self->tail != NULL)
    self->tail->next = new_block;
  else
    self->head = new_block;
  self->tail = new_block;

  return _M_cast(uint8_t*, new_block) + sizeof(_DynamicArenaBlock);
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS_ARENAS_DYNAMIC_H
