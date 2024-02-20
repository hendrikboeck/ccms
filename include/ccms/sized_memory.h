#ifndef __CCMS__SIZED_MEMORY__H
#define __CCMS__SIZED_MEMORY__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ccms/_defs.h"
#include "ccms/_macros.h"
#include "ccms/box.h"

/**
 * @typedef SizedMemory
 * @brief Typedef for struct SizedMemory
 *
 * This typedef provides a shorthand for the struct SizedMemory.
 */
typedef struct SizedMemory SizedMemory;

/**
 * @struct SizedMemory
 * @brief A structure representing a sized memory.
 *
 * This structure represents a sized memory with a pointer to its data and the
 * size of the data. It is used to represent a block of memory with a known
 * size. It is layed out in memory as follows: [ SizedMemory | data ]
 *
 * @var SizedMemory::ptr
 * The pointer to the data of the sized memory. It is a pointer to a uint8_t.
 *
 * @var SizedMemory::size
 * The size of the data of the sized memory. It is of type size_t.
 */
struct SizedMemory {
  uint8_t* ptr;
  size_t size;
};

/**
 * @brief Creates a new SizedMemory object.
 *
 * This function creates a new SizedMemory object and allocates memory for it.
 * The size of the allocated memory is the size of the SizedMemory struct plus
 * the size specified by the parameter.
 *
 * @param size The size of the memory to be allocated
 *
 * @return A pointer to the newly created SizedMemory object.
 */
__CCMS__INLINE
SizedMemory* sized_memory__new(const size_t size) {
  SizedMemory* self =
      _M_cast(SizedMemory*, _M_alloc(sizeof(SizedMemory) + size));

  self->ptr = _M_cast(uint8_t*, self) + sizeof(SizedMemory);
  self->size = size;

  return self;
}

/**
 * @brief Frees the memory allocated to a SizedMemory object.
 *
 * This function frees the memory allocated to a SizedMemory object.
 *
 * @param self The SizedMemory object to free the memory of.
 */
__CCMS__INLINE
void sized_memory__free(SizedMemory* self) {
  _M_free(self);
}

/**
 * @brief Clones a SizedMemory object.
 *
 * This function clones a SizedMemory object, creating a new SizedMemory with
 * the same pointer and size as the original.
 *
 * @param self The SizedMemory object to clone.
 *
 * @return A new SizedMemory object with the same pointer and size as the
 * original.
 */
__CCMS__INLINE
SizedMemory* sized_memory__clone(const SizedMemory* self) {
  SizedMemory* other = sized_memory__new(self->size);

  memcpy(other->ptr, self->ptr, self->size);

  return other;
}

/**
 * @brief Returns a Box object representing the contained memory of a
 * SizedMemory object.
 *
 * This function takes a SizedMemory object and returns a Box object that
 * represents the memory contained in the SizedMemory object.
 *
 * @param self A pointer to the SizedMemory object.
 *
 * @return A Box object representing the memory contained in the SizedMemory
 * object.
 */
__CCMS__INLINE
Box sized_memory__as_box(const SizedMemory* self) {
  return box__ctor(self->ptr, self->size);
}

/**
 * @brief Constructs a SizedMemory object from a Box object.
 *
 * This function constructs a SizedMemory object from a Box object. It allocates
 * memory for the SizedMemory object and copies the data from the Box object to
 * the SizedMemory object.
 *
 * @param box The Box object to construct the SizedMemory object from.
 *
 * @return A pointer to the newly created SizedMemory object.
 */
__CCMS__INLINE
SizedMemory* sized_memory__from_box(const Box box) {
  SizedMemory* self = sized_memory__new(box.size);

  memcpy(self->ptr, box.ptr, box.size);

  return self;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__SIZED_MEMORY__H
