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
 * @typedef Box
 * @brief Typedef for struct Box
 *
 * This typedef provides a shorthand for the struct Box.
 */
typedef struct Box Box;

/**
 * @struct Box
 * @brief A structure representing a box.
 *
 * This structure represents a box with a pointer to its data and the size of
 * the data in bytes.
 *
 * @var Box::ptr
 * The pointer to the data of the box. It is a pointer to a uint8_t.
 *
 * @var Box::size
 * The size of the data of the box in bytes. It is of type size_t.
 */
struct Box {
  uint8_t* ptr;
  size_t size;
};

/**
 * @brief Constructs a Box object.
 *
 * This function constructs a Box object with the given pointer and size.
 *
 * @param ptr The pointer to the data of the box.
 * @param size The size of the data of the box.
 *
 * @return A Box object.
 */
__CCMS__INLINE
Box box__ctor(uint8_t* ptr, size_t size) {
  return (Box){ptr, size};
}

/**
 * @brief Clones a Box object.
 *
 * This function clones a Box object, creating a new Box with the same pointer
 * and size as the original.
 *
 * @param other The Box object to clone.
 *
 * @return A new Box object with the same pointer and size as the original.
 */
__CCMS__INLINE
Box box__clone(const Box* other) {
  return (Box){other->ptr, other->size};
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__BOX__H
