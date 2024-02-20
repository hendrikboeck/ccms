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
// ------------------ _DynamicArenaBlock ------------------
//
//

void test___dynamic_arena_block__new() {
  // -- TEST
  _DynamicArenaBlock* block = _dynamic_arena_block__new(10, NULL);
  assert(block != NULL);
  assert(block->size == 10);
  assert(block->next == NULL);

  // -- CLEANUP
  _dynamic_arena_block__free(block);
}

void test___dynamic_arena_block__clone() {
  // -- PREPARE
  _DynamicArenaBlock* block = _dynamic_arena_block__new(10, NULL);

  // -- TEST
  _DynamicArenaBlock* clone = _dynamic_arena_block__clone(block);
  assert(clone != NULL);
  assert(clone->size == block->size);
  assert(clone->next == block->next);

  // -- CLEANUP
  _dynamic_arena_block__free(block);
  _dynamic_arena_block__free(clone);
}

//
//
// ------------------ DynamicArena ------------------
//
//

void test__dynamic_arena__new() {
  // -- TEST
  DynamicArena* arena = dynamic_arena__new();
  assert(arena != NULL);
  assert(arena->head == NULL);
  assert(arena->tail == NULL);

  // -- CLEANUP
  dynamic_arena__free(arena);
}

void test__dynamic_arena__reset() {
  // -- PREPARE
  DynamicArena* arena = dynamic_arena__new();

  // -- TEST
  dynamic_arena__reset(arena);
  assert(arena->head == NULL);
  assert(arena->tail == NULL);

  // -- CLEANUP
  dynamic_arena__free(arena);
}

void test__dynamic_arena__alloc() {
  // -- PREPARE
  DynamicArena* arena = dynamic_arena__new();

  // -- TEST
  uint8_t* memory = dynamic_arena__alloc(arena, 10);
  assert(memory != NULL);

  // -- CLEANUP
  dynamic_arena__free(arena);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- _DynamicArenaBlock
  test___dynamic_arena_block__new();
  test___dynamic_arena_block__clone();

  // -- DynamicArena
  test__dynamic_arena__new();
  test__dynamic_arena__reset();
  test__dynamic_arena__alloc();

  return 0;
}