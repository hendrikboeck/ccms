#ifndef __CCMS___MACROS__H
#define __CCMS___MACROS__H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def _M_cast(T, expr)
 * @brief A macro that casts an expression to a specified type.
 *
 * @param T The type to cast to.
 * @param expr The expression to be cast.
 */
#define _M_cast(T, expr) ((T)(expr))

/**
 * @def _M_addr(expr)
 * @brief A macro that gets the memory address of an expression.
 *
 * @param expr The expression to get the address of.
 */
#define _M_addr(expr) (&(expr))

#ifndef _M_alloc
#include <stdlib.h>
/**
 * @def _M_alloc(size)
 * @brief A macro that allocates memory of a specified size in bytes.
 *
 * @param size The size of the memory to be allocated in bytes.
 */
#define _M_alloc(size) malloc(size)
#endif

#ifndef _M_free
#include <stdlib.h>
/**
 * @def _M_free(ptr)
 * @brief A macro that frees the memory allocated to a pointer.
 *
 * @param ptr The pointer to free the memory of.
 */
#define _M_free(ptr) free(ptr)
#endif

/**
 * @def _M_new(T)
 * @brief A macro that allocates memory for a new object of type T.
 *
 * @param T The type of the object to allocate memory for.
 */
#define _M_new(T) ((T*)_M_alloc(sizeof(T)))

/**
 * @def _M_new_arr(T, len)
 * @brief A macro that allocates memory for a new array of objects of type T.
 *
 * @param T The type of the objects to allocate memory for.
 * @param len The length of the array to allocate memory for.
 */
#define _M_new_arr(T, len) ((T*)_M_alloc(sizeof(T) * len))

#ifndef __CCMS_NO_SIZE_MACROS

#define KiB(n) ((n)*1024)
#define MiB(n) (KiB(n) * 1024)
#define GiB(n) (MiB(n) * 1024)
#define TiB(n) (GiB(n) * 1024)

#define KB(n) ((n)*1000)
#define MB(n) (KB(n) * 1000)
#define GB(n) (MB(n) * 1000)
#define TB(n) (GB(n) * 1000)

#endif  // __CCMS_NO_SIZE_MACROS

#ifdef __cplusplus
}
#endif

#endif  // __CCMS___MACROS__H
