#ifndef __CCMS___MACROS__H
#define __CCMS___MACROS__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define _M_cast(T, expr) ((T)(expr))

#define _M_addr(expr) (&(expr))

#ifndef _M_alloc
#define _M_alloc(size) malloc(size)
#endif

#ifndef _M_free
#define _M_free(expr) free(expr)
#endif

#define _M_new(T) ((T *)_M_alloc(sizeof(T)))
#define _M_new_arr(T, len) ((T *)_M_alloc(sizeof(T) * len))

#ifndef __CCMS_NO_SIZE_MACROS

#define KiB(n) ((n) * 1024)
#define MiB(n) (KiB(n) * 1024)
#define GiB(n) (MiB(n) * 1024)
#define TiB(n) (GiB(n) * 1024)

#define KB(n) ((n) * 1000)
#define MB(n) (KB(n) * 1000)
#define GB(n) (MB(n) * 1000)
#define TB(n) (GB(n) * 1000)

#endif // __CCMS_NO_SIZE_MACROS

#ifdef __cplusplus
}
#endif

#endif // __CCMS___MACROS__H
