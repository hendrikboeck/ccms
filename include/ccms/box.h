#ifndef __CCMS__BOX__H
#define __CCMS__BOX__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct Box Box;

struct Box {
  uint8_t *ptr;
  size_t size;
};

static inline Box box__ctor(uint8_t *ptr, size_t size) {
  return (Box){ptr, size};
}

static inline Box box__clone(const Box *other) {
  return (Box){other->ptr, other->size};
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS__BOX__H
