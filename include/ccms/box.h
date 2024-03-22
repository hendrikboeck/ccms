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

#ifndef __CCMS__BOX__H
#define __CCMS__BOX__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "ccms/_defs.h"

/**
 * @typedef box_t
 * @brief Typedef for struct box_t
 *
 * This typedef provides a shorthand for the struct box_t.
 */
typedef struct box_t box_t;

/**
 * @struct box_t
 * @brief A structure representing a box.
 *
 * This structure represents a box with a pointer to its data and the size of
 * the data in bytes.
 *
 * @var box_t::ptr
 * The pointer to the data of the box. It is a pointer to a uint8_t.
 *
 * @var box_t::size
 * The size of the data of the box in bytes. It is of type size_t.
 */
struct box_t {
  uint8_t* ptr;
  size_t size;
};

/**
 * @brief Constructs a box_t object.
 *
 * This function constructs a box_t object with the given pointer and size.
 *
 * @param ptr The pointer to the data of the box.
 * @param size The size of the data of the box.
 *
 * @return A box_t object.
 */
__CCMS__INLINE
box_t box__ctor(uint8_t* ptr, size_t size) {
  return (box_t){.ptr = ptr, .size = size};
}

/**
 * @brief Clones a box_t object.
 *
 * This function clones a box_t object, creating a new box_t with the same
 * pointer and size as the original.
 *
 * @param other The box_t object to clone.
 *
 * @return A new box_t object with the same pointer and size as the original.
 */
__CCMS__INLINE
box_t box__clone(const box_t* other) {
  return (box_t){.ptr = other->ptr, .size = other->size};
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__BOX__H
