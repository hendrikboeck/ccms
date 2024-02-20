#undef NDEBUG
#include <assert.h>

#include "ccms/arenas/static.h"

void test__static_arena__new_and_free() {
  StaticArena* sa = static_arena__new(10);
  assert(sa != NULL);
  assert(sa->size == 10);
  static_arena__free(sa);
}

void test__static_arena__clone() {
  StaticArena* sa1 = static_arena__new(10);
  StaticArena* sa2 = static_arena__clone(sa1);
  assert(sa2 != NULL);
  assert(sa2->size == sa1->size);
  static_arena__free(sa1);
  static_arena__free(sa2);
}

void test__static_arena__cap() {
  StaticArena* sa = static_arena__new(10);
  assert(static_arena__cap(sa) == 10);
  static_arena__free(sa);
}

void test__static_arena__reset() {
  StaticArena* sa = static_arena__new(10);
  static_arena__alloc(sa, 5);
  assert(static_arena__cap(sa) == 5);
  static_arena__reset(sa);
  assert(static_arena__cap(sa) == 10);
  static_arena__free(sa);
}

void test__static_arena__alloc() {
  StaticArena* sa = static_arena__new(10);
  uint8_t* mem = static_arena__alloc(sa, 5);
  assert(mem != NULL);
  assert(static_arena__cap(sa) == 6);
  static_arena__free(sa);
}

int main() {
  test__static_arena__new_and_free();
  test__static_arena__clone();
  test__static_arena__cap();
  test__static_arena__reset();
  test__static_arena__alloc();
  return 0;
}