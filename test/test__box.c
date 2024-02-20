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
#include "ccms/box.h"

//
//
// ------------------ Box ------------------
//
//

void test__box__ctor() {
  // -- PREPARE
  uint8_t data = 5;

  // -- TEST
  Box b = box__ctor(&data, sizeof(data));
  assert(*b.ptr == 5);
  assert(b.size == sizeof(data));
}

void test__box__clone() {
  // -- PREPARE
  uint8_t data = 5;
  Box b1 = box__ctor(&data, sizeof(data));

  // -- TEST
  Box b2 = box__clone(&b1);
  assert(b2.ptr == b1.ptr);
  assert(b2.size == b1.size);
}

//
//
// ------------------ main ------------------
//
//

int main() {
  // -- Box
  test__box__ctor();
  test__box__clone();

  return 0;
}