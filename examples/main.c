#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define __CCMS__SUPPRESS_WARNINGS
#define __CCMS__NO_SIZE_MACROS

void* my_malloc(const size_t size) {
  void* ptr = malloc(size);
  printf("malloc: %p, size %ldB\n", ptr, size);
  return ptr;
}

void my_free(void* ptr) {
  printf("free  : %p\n", ptr);
  free(ptr);
}

#define _M_alloc(size) my_malloc(size)
#define _M_free(expr) my_free(expr)

#include "ccms/_macros.h"
#include "ccms/arena/paged.h"

int32_t main(void) {
  pg_arena_t* arena = pg_arena__new(sizeof(int32_t) * 10);

  int32_t* arr0 =
      _M_cast(int32_t*, pg_arena__alloc(arena, sizeof(int32_t) * 10));
  assert(arr0 != NULL);

  int32_t* arr1 =
      _M_cast(int32_t*, pg_arena__alloc(arena, sizeof(int32_t) * 16));
  assert(arr1 == NULL);

  pg_arena__free(arena);
  return EXIT_SUCCESS;
}
