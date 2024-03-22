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

// do not move or delete this #undef, otherwise the test will always pass, as
// assert is only defined in debug mode. This #undef forces assert to be defined
#undef NDEBUG
#include <assert.h>

// Include the header file to test
#include "ccms/arena/dynamic.h"

//
//
// ------------------ _dyn_arena_block_t ------------------
//
//

void test___dyn_arena_block__new() {
  // -- TEST
  _dyn_arena_block_t* block = _dyn_arena_block__new(10, NULL);
  assert(block != NULL);
  assert(block->size == 10);
  assert(block->next == NULL);

  // -- CLEANUP
  _dyn_arena_block__free(block);
}

void test___dyn_arena_block__clone() {
  // -- PREPARE
  _dyn_arena_block_t* block = _dyn_arena_block__new(10, NULL);

  // -- TEST
  _dyn_arena_block_t* clone = _dyn_arena_block__clone(block);
  assert(clone != NULL);
  assert(clone->size == block->size);
  assert(clone->next == block->next);

  // -- CLEANUP
  _dyn_arena_block__free(block);
  _dyn_arena_block__free(clone);
}

//
//
// ------------------ dyn_arena_t ------------------
//
//

void test__dyn_arena__new() {
  // -- TEST
  dyn_arena_t* arena = dyn_arena__new();
  assert(arena != NULL);
  assert(arena->head == NULL);
  assert(arena->tail == NULL);

  // -- CLEANUP
  dyn_arena__free(arena);
}

void test__dyn_arena__reset() {
  // -- PREPARE
  dyn_arena_t* arena = dyn_arena__new();

  // -- TEST
  dyn_arena__reset(arena);
  assert(arena->head == NULL);
  assert(arena->tail == NULL);

  // -- CLEANUP
  dyn_arena__free(arena);
}

void test__dyn_arena__alloc() {
  // -- PREPARE
  dyn_arena_t* arena = dyn_arena__new();

  // -- TEST
  uint8_t* memory = dyn_arena__alloc(arena, 10);
  assert(memory != NULL);

  // -- CLEANUP
  dyn_arena__free(arena);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- _dyn_arena_block_t
  test___dyn_arena_block__new();
  test___dyn_arena_block__clone();

  // -- dyn_arena_t
  test__dyn_arena__new();
  test__dyn_arena__reset();
  test__dyn_arena__alloc();

  return 0;
}