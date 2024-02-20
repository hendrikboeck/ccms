#undef NDEBUG
#include <assert.h>

#include "ccms/box.h"

void test__box__ctor() {
  uint8_t data = 5;
  Box b = box__ctor(&data, sizeof(data));
  assert(*b.ptr == 5);
  assert(b.size == sizeof(data));
}

void test__box__clone() {
  uint8_t data = 5;
  Box b1 = box__ctor(&data, sizeof(data));
  Box b2 = box__clone(&b1);
  assert(b2.ptr == b1.ptr);
  assert(b2.size == b1.size);
}

int main() {
  test__box__ctor();
  test__box__clone();
  return 0;
}