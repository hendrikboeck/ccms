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

#ifndef __CCMS__SIZED_MEM__H
#define __CCMS__SIZED_MEM__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_defs.h"
#include "ccms/_macros.h"
#include "ccms/box.h"

/**
 * @typedef sized_mem_t
 * @brief Typedef for struct sized_mem_t
 *
 * This typedef provides a shorthand for the struct sized_mem_t.
 */
typedef struct sized_mem_t sized_mem_t;

/**
 * @struct sized_mem_t
 * @brief A structure representing a sized memory.
 *
 * This structure represents a sized memory with a pointer to its data and the
 * size of the data. It is used to represent a block of memory with a known
 * size. It is layed out in memory as follows: [ sized_mem_t | data ]
 *
 * @var sized_mem_t::ptr
 * The pointer to the data of the sized memory. It is a pointer to a uint8_t.
 *
 * @var sized_mem_t::size
 * The size of the data of the sized memory. It is of type size_t.
 */
struct sized_mem_t {
  uint8_t* ptr;
  size_t size;
};

/**
 * @brief Creates a new sized_mem_t object.
 *
 * This function creates a new sized_mem_t object and allocates memory for it.
 * The size of the allocated memory is the size of the sized_mem_t struct plus
 * the size specified by the parameter.
 *
 * @param size The size of the memory to be allocated
 *
 * @return A pointer to the newly created sized_mem_t object.
 */
__CCMS__INLINE
sized_mem_t* sized_mem__new(const size_t size) {
  sized_mem_t* self =
      _M_cast(sized_mem_t*, _M_alloc(sizeof(sized_mem_t) + size));

  self->ptr = _M_cast(uint8_t*, self) + sizeof(sized_mem_t);
  self->size = size;

  return self;
}

/**
 * @brief Frees the memory allocated to a sized_mem_t object.
 *
 * This function frees the memory allocated to a sized_mem_t object.
 *
 * @param self The sized_mem_t object to free the memory of.
 */
__CCMS__INLINE
void sized_mem__free(sized_mem_t* self) {
  _M_free(self);
}

/**
 * @brief Clones a sized_mem_t object.
 *
 * This function clones a sized_mem_t object, creating a new sized_mem_t with
 * the same pointer and size as the original.
 *
 * @param self The sized_mem_t object to clone.
 *
 * @return A new sized_mem_t object with the same pointer and size as the
 * original.
 */
__CCMS__INLINE
sized_mem_t* sized_mem__clone(const sized_mem_t* self) {
  sized_mem_t* other = sized_mem__new(self->size);

  memcpy(other->ptr, self->ptr, self->size);

  return other;
}

/**
 * @brief Returns a box_t object representing the contained memory of a
 * sized_mem_t object.
 *
 * This function takes a sized_mem_t object and returns a box_t object that
 * represents the memory contained in the sized_mem_t object.
 *
 * @param self A pointer to the sized_mem_t object.
 *
 * @return A box_t object representing the memory contained in the sized_mem_t
 * object.
 */
__CCMS__INLINE
box_t sized_mem__as_box(const sized_mem_t* self) {
  return box__ctor(self->ptr, self->size);
}

/**
 * @brief Constructs a sized_mem_t object from a box_t object.
 *
 * This function constructs a sized_mem_t object from a box_t object. It
 * allocates memory for the sized_mem_t object and copies the data from the
 * box_t object to the sized_mem_t object.
 *
 * @param box The box_t object to construct the sized_mem_t object from.
 *
 * @return A pointer to the newly created sized_mem_t object.
 */
__CCMS__INLINE
sized_mem_t* sized_mem__from_box(const box_t box) {
  sized_mem_t* self = sized_mem__new(box.size);

  memcpy(self->ptr, box.ptr, box.size);

  return self;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__SIZED_MEM__H
