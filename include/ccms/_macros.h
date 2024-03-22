/******************************************************************************/
/* CCMS - Collection of C Memory Structures, a lightweight header-only C      */
/* library.                                                                   */
/* Copyright (C) 2024, Hendrik Boeck <hendrikboeck.dev@protonmail.com>        */
/*                                                                            */
/* This program is free software: you can redistribute it and/or modify  it   */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation, either version 3 of the License, or (at your option)  */
/* any later version.                                                         */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but        */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY */
/* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   */
/* for more details.                                                          */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program.  If not, see <https://www.gnu.org/licenses/>.           */
/******************************************************************************/

#ifndef __CCMS___MACROS__H
#define __CCMS___MACROS__H

#ifdef __cplusplus
extern "C" {
#endif

#define _M_cast(T, expr) ((T)(expr))

#define _M_addr(expr) (&(expr))

#ifndef _M_alloc
#include <stdlib.h>

#define _M_alloc(size) malloc(size)
#endif

#ifndef _M_free
#include <stdlib.h>

#define _M_free(ptr) free(ptr)
#endif

#define _M_new(T) _M_cast(T*, _M_alloc(sizeof(T)))

#define _M_new_arr(T, len) _M_cast(T*, _M_alloc(sizeof(T) * len))

#ifndef __CCMS__NO_SIZE_MACROS

#define KiB(n) ((n) * 1024)
#define MiB(n) (KiB(n) * 1024)
#define GiB(n) (MiB(n) * 1024)
#define TiB(n) (GiB(n) * 1024)

#define KB(n) ((n) * 1000)
#define MB(n) (KB(n) * 1000)
#define GB(n) (MB(n) * 1000)
#define TB(n) (GB(n) * 1000)

#endif  // __CCMS__NO_SIZE_MACROS

#ifdef __cplusplus
}
#endif

#endif  // __CCMS___MACROS__H
