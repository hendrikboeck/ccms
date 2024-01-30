#ifndef __CCMS__BOX__H
#define __CCMS__BOX__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

struct Box {
  uint8_t *ptr;
  size_t size;
};
typedef struct Box Box;

extern inline Box box__ctor(uint8_t *ptr, size_t size) {
  return (Box){ptr, size};
}

extern inline Box box__clone(const Box *other) {
  return (Box){other->ptr, other->size};
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS__BOX__H
