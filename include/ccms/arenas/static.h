#ifndef __CCMS__ARENAS__STATIC__H
#define __CCMS__ARENAS__STATIC__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef __CCMS__SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_defs.h"
#include "ccms/_macros.h"

/**
 * @typedef StaticArena
 * @brief Typedef for struct StaticArena
 *
 * This typedef provides a shorthand for the struct StaticArena.
 */
typedef struct StaticArena StaticArena;

/**
 * @struct StaticArena
 * @brief A structure representing a static memory arena.
 *
 * This structure represents a static memory arena with a write head pointer and
 * the size of the arena.
 *
 * @var StaticArena::writehead
 * The write head pointer of the static memory arena. It is a pointer to a
 * uint8_t.
 *
 * @var StaticArena::size
 * The size of the static memory arena. It is of type size_t.
 */
struct StaticArena {
  uint8_t* writehead;
  size_t size;
};

/**
 * @brief Creates a new StaticArena object.
 *
 * This function creates a new StaticArena object and allocates memory for it.
 * The size of the allocated memory is the size of the StaticArena struct plus
 * the size specified by the parameter.
 *
 * @param size The size of the memory to be allocated
 *
 * @return A pointer to the newly created StaticArena object.
 */
__CCMS__INLINE
StaticArena* static_arena__new(const size_t size) {
  StaticArena* self =
      _M_cast(StaticArena*, _M_alloc(sizeof(StaticArena) + size));

  self->writehead = _M_cast(uint8_t*, self) + sizeof(StaticArena);
  self->size = size;

  return self;
}

/**
 * @brief Frees the memory allocated to a StaticArena object.
 *
 * This function frees the memory allocated to a StaticArena object.
 *
 * @param self The StaticArena object to free the memory of.
 */
__CCMS__INLINE
void static_arena__free(StaticArena* self) {
  _M_free(self);
}

/**
 * @brief Clones a StaticArena object.
 *
 * This function clones a StaticArena object, creating a new StaticArena with
 * the same relative write head pointer position and size as the original.
 *
 * @param self The StaticArena object to clone.
 *
 * @return A new StaticArena object with the same relative write head pointer
 * position and size as the original.
 */
__CCMS__INLINE
StaticArena* static_arena__clone(const StaticArena* self) {
  StaticArena* other = static_arena__new(self->size);
  size_t wh_offset = _M_cast(size_t, self->writehead - _M_cast(uint8_t*, self));

  memcpy(other, self, self->size + sizeof(StaticArena));
  other->writehead = _M_cast(uint8_t*, other) + wh_offset;

  return other;
}

/**
 * @brief Gets the available capacity of a StaticArena object.
 *
 * This function gets the available capacity of a StaticArena object, which is
 * the amount of free memory in the arena.
 *
 * @param self The StaticArena object to get the available capacity of.
 *
 * @return The available capacity of the StaticArena object.
 */
__CCMS__INLINE
size_t static_arena__cap(const StaticArena* self) {
  return (self->size + sizeof(StaticArena)) -
         _M_cast(size_t, self->writehead - _M_cast(uint8_t*, self));
}

/**
 * @brief Resets the StaticArena object.
 *
 * @param self The StaticArena object to reset.
 */
__CCMS__INLINE
void static_arena__reset(StaticArena* self) {
  self->writehead = _M_cast(uint8_t*, self) + sizeof(StaticArena);
}

/**
 * @brief Allocates a chunk of memory from a StaticArena object.
 *
 * This function allocates a chunk of memory of a specified size from a
 * StaticArena object.
 *
 * @param self The StaticArena object to allocate memory from.
 * @param size The size of the memory to be allocated in bytes.
 *
 * @return A pointer to the allocated memory.
 */
__CCMS__INLINE
uint8_t* static_arena__alloc(StaticArena* self, const size_t size) {
  if (static_arena__cap(self) < size) {
#ifndef __CCMS__SUPPRESS_WARNINGS
    fprintf(stderr,
            "warning: tried to allocate a chunk of memory of size %ld from an "
            "arena (static) with only %ld free memory, returned NULL\n",
            size, static_arena__cap(self));
#endif
    return NULL;
  }

  uint8_t* result = self->writehead;
  self->writehead += size;

  return result;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS_ARENAS_STATIC_H
