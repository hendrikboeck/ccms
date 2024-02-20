#undef NDEBUG
#include <assert.h>

#include "ccms/sized_memory.h"

void test__sized_memory__new() {
  SizedMemory* sm = sized_memory__new(10);
  assert(sm != NULL);
  assert(sm->size == 10);
  sized_memory__free(sm);
}

void test__sized_memory__clone() {
  SizedMemory* sm1 = sized_memory__new(10);
  SizedMemory* sm2 = sized_memory__clone(sm1);
  assert(sm2 != NULL);
  assert(sm2->size == sm1->size);
  assert(memcmp(sm1->ptr, sm2->ptr, sm1->size) == 0);
  sized_memory__free(sm1);
  sized_memory__free(sm2);
}

void test__sized_memory__as_box() {
  SizedMemory* sm = sized_memory__new(10);
  Box b = sized_memory__as_box(sm);
  assert(b.ptr == sm->ptr);
  assert(b.size == sm->size);
  sized_memory__free(sm);
}

void test__sized_memory__from_box() {
  Box b = box__ctor((uint8_t*)"hello", 5);
  SizedMemory* sm = sized_memory__from_box(b);
  assert(sm != NULL);
  assert(sm->size == b.size);
  assert(memcmp(sm->ptr, b.ptr, b.size) == 0);
  sized_memory__free(sm);
}

int main() {
  test__sized_memory__new();
  test__sized_memory__clone();
  test__sized_memory__as_box();
  test__sized_memory__from_box();
  return 0;
}