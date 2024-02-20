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
#include "ccms/sized_memory.h"

//
//
// ------------------ SizedMemory ------------------
//
//

void test__sized_memory__new() {
  // -- TEST
  SizedMemory* sm = sized_memory__new(10);
  assert(sm != NULL);
  assert(sm->size == 10);

  // -- CLEANUP
  sized_memory__free(sm);
}

void test__sized_memory__clone() {
  // -- PREPARE
  SizedMemory* sm1 = sized_memory__new(10);

  // -- TEST
  SizedMemory* sm2 = sized_memory__clone(sm1);
  assert(sm2 != NULL);
  assert(sm2->size == sm1->size);
  assert(memcmp(sm1->ptr, sm2->ptr, sm1->size) == 0);

  // -- CLEANUP
  sized_memory__free(sm1);
  sized_memory__free(sm2);
}

void test__sized_memory__as_box() {
  // -- PREPARE
  SizedMemory* sm = sized_memory__new(10);

  // -- TEST
  Box b = sized_memory__as_box(sm);
  assert(b.ptr == sm->ptr);
  assert(b.size == sm->size);

  // -- CLEANUP
  sized_memory__free(sm);
}

void test__sized_memory__from_box() {
  // -- PREPARE
  Box b = box__ctor((uint8_t*)"hello", 5);

  // -- TEST
  SizedMemory* sm = sized_memory__from_box(b);
  assert(sm != NULL);
  assert(sm->size == b.size);
  assert(memcmp(sm->ptr, b.ptr, b.size) == 0);

  // -- CLEANUP
  sized_memory__free(sm);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- SizedMemory
  test__sized_memory__new();
  test__sized_memory__clone();
  test__sized_memory__as_box();
  test__sized_memory__from_box();

  return 0;
}