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
#include "ccms/arena/paged.h"

//
//
// ------------------ _pg_arena_page_t ------------------
//
//

void test___pg_arena_page__new() {
  // -- TEST
  _pg_arena_page_t* page = _pg_arena_page__new(10, NULL);
  assert(page != NULL);
  assert(page->pos == 0);
  assert(page->next == NULL);

  // -- CLEANUP
  _pg_arena_page__free(page);
}

void test___pg_arena_page__reset() {
  // -- PREPARE
  _pg_arena_page_t* page = _pg_arena_page__new(10, NULL);
  page->pos = 5;

  // -- TEST
  _pg_arena_page__reset(page);
  assert(page->pos == 0);

  // -- CLEANUP
  _pg_arena_page__free(page);
}

//
//
// ------------------ pg_arena_t ------------------
//
//

void test__pg_arena__new() {
  // -- TEST
  pg_arena_t* arena = pg_arena__new(10);
  assert(arena != NULL);
  assert(arena->page_size == 10);
  assert(arena->head != NULL);
  assert(arena->tail != NULL);

  // -- CLEANUP
  pg_arena__free(arena);
}

void test__pg_arena__reset() {
  // -- PREPARE
  pg_arena_t* arena = pg_arena__new(10);
  uint8_t* chunk = pg_arena__alloc(arena, 5);

  // -- TEST
  pg_arena__reset(arena);
  assert(arena->tail == arena->head);

  for (_pg_arena_page_t* itr = arena->head; itr != NULL; itr = itr->next)
    assert(itr->pos == 0);

  // -- CLEANUP
  pg_arena__free(arena);
}

void test__pg_arena__hard_reset() {
  // -- PREPARE
  pg_arena_t* arena = pg_arena__new(10);
  uint8_t* chunk = pg_arena__alloc(arena, 5);

  // -- TEST
  pg_arena__hard_reset(arena);
  assert(arena->tail == arena->head);
  assert(arena->head->next == NULL);

  for (_pg_arena_page_t* itr = arena->head; itr != NULL; itr = itr->next)
    assert(itr->pos == 0);

  // -- CLEANUP
  pg_arena__free(arena);
}

void test__pg_arena__alloc() {
  // -- PREPARE
  pg_arena_t* arena = pg_arena__new(10);

  // -- TEST
  uint8_t* chunk = pg_arena__alloc(arena, 5);
  assert(chunk != NULL);

  // -- CLEANUP
  pg_arena__free(arena);
}

void test__pg_arena__avg_util() {
  // -- PREPARE
  pg_arena_t* arena = pg_arena__new(10);
  uint8_t* chunk = pg_arena__alloc(arena, 5);

  // -- TEST
  float avg_util = pg_arena__avg_util(arena);
  assert(avg_util == 0.5);

  // -- CLEANUP
  pg_arena__free(arena);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- _pg_arena_page_t
  test___pg_arena_page__new();
  test___pg_arena_page__reset();

  // -- pg_arena_t
  test__pg_arena__new();
  test__pg_arena__reset();
  test__pg_arena__hard_reset();
  test__pg_arena__alloc();
  test__pg_arena__avg_util();

  return 0;
}