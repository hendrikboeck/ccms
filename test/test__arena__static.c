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
#include "ccms/arena/static.h"

//
//
// ------------------ StaticArena ------------------
//
//

void test__static_arena__new_and_free() {
  // -- TEST
  StaticArena* sa = static_arena__new(10);
  assert(sa != NULL);
  assert(sa->size == 10);

  // -- CLEANUP
  static_arena__free(sa);
}

void test__static_arena__clone() {
  // -- PREPARE
  StaticArena* sa1 = static_arena__new(10);

  // -- TEST
  StaticArena* sa2 = static_arena__clone(sa1);
  assert(sa2 != NULL);
  assert(sa2->size == sa1->size);

  // -- CLEANUP
  static_arena__free(sa1);
  static_arena__free(sa2);
}

void test__static_arena__cap() {
  // -- PREPARE
  StaticArena* sa = static_arena__new(10);

  // -- TEST
  assert(static_arena__cap(sa) == 10);

  // -- CLEANUP
  static_arena__free(sa);
}

void test__static_arena__reset() {
  // -- PREPARE
  StaticArena* sa = static_arena__new(10);
  static_arena__alloc(sa, 5);
  assert(static_arena__cap(sa) == 5);

  // -- TEST
  static_arena__reset(sa);
  assert(static_arena__cap(sa) == 10);

  // -- CLEANUP
  static_arena__free(sa);
}

void test__static_arena__alloc() {
  // -- PREPARE
  StaticArena* sa = static_arena__new(10);

  // -- TEST
  uint8_t* mem = static_arena__alloc(sa, 5);
  assert(mem != NULL);
  assert(static_arena__cap(sa) == 5);

  // -- CLEANUP
  static_arena__free(sa);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- StaticArena
  test__static_arena__new_and_free();
  test__static_arena__clone();
  test__static_arena__cap();
  test__static_arena__reset();
  test__static_arena__alloc();

  return 0;
}