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

#ifndef __CCMS__ARENAS__PAGED__H
#define __CCMS__ARENAS__PAGED__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#ifndef __CCMS__SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_defs.h"
#include "ccms/_macros.h"

/**
 * @typedef _PagedArenaPage
 * @brief A typedef for the _PagedArenaPage structure.
 *
 * This typedef provides a shorthand for the struct _PagedArenaPage.
 */
typedef struct _PagedArenaPage _PagedArenaPage;

/**
 * @typedef PagedArena
 * @brief A typedef for the PagedArena structure.
 *
 * This typedef provides a shorthand for the struct PagedArena.
 */
typedef struct PagedArena PagedArena;

/**
 * @struct _PagedArenaPage
 * @brief A structure representing a page in a paged memory arena.
 *
 * This structure represents a page in a paged memory arena with a pointer to
 * the next page and the position within the current page.
 *
 * @var _PagedArenaPage::next
 * The pointer to the next page in the paged memory arena. It is a pointer to
 * another _PagedArenaPage.
 *
 * @var _PagedArenaPage::pos
 * The position within the current page. It is of type size_t.
 */
struct _PagedArenaPage {
  _PagedArenaPage* next;
  size_t pos;
};

/**
 * @brief Creates a new _PagedArenaPage object.
 *
 * This function creates a new _PagedArenaPage object and allocates memory for
 * it. The size of the allocated memory is the size of the _PagedArenaPage
 * struct plus the size specified by the parameter. The pos variable is
 * initialized to 0, representing the index of the first unoccupied byte in the
 * page.
 *
 * @param size The size of the memory to be allocated in addition to the size of
 * the _PagedArenaPage struct.
 * @param next The pointer to the next _PagedArenaPage object.
 *
 * @return A pointer to the newly created _PagedArenaPage object.
 */
__CCMS__INLINE
_PagedArenaPage* _paged_arena_page__new(const size_t size,
                                        _PagedArenaPage* next) {
  _PagedArenaPage* self =
      _M_cast(_PagedArenaPage*, _M_alloc(sizeof(_PagedArenaPage) + size));

  self->pos = 0;
  self->next = next;

  return self;
}

/**
 * @brief Frees the memory allocated for a _PagedArenaPage object.
 *
 * This function frees the memory allocated for a _PagedArenaPage object.
 *
 * @param self A pointer to the _PagedArenaPage object to be freed.
 */
__CCMS__INLINE
void _paged_arena_page__free(_PagedArenaPage* self) {
  _M_free(self);
}

/**
 * @brief Resets the position of a _PagedArenaPage object.
 *
 * This function resets the position of a _PagedArenaPage object to 0,
 * indicating that the page is unoccupied.
 *
 * @param self A pointer to the _PagedArenaPage object to be reset.
 */
__CCMS__INLINE
void _paged_arena_page__reset(_PagedArenaPage* self) {
  self->pos = 0;
}

/**
 * @struct PagedArena
 * @brief A structure representing a paged memory arena.
 *
 * This structure represents a paged memory arena with a head and tail pointer
 * to _PagedArenaPage, and the size of each page.
 *
 * @var PagedArena::head
 * The head pointer to the first _PagedArenaPage in the paged memory arena.
 *
 * @var PagedArena::tail
 * The tail pointer to the last _PagedArenaPage in the paged memory arena.
 *
 * @var PagedArena::page_size
 * The size of each page in the paged memory arena. It is of type size_t.
 */
struct PagedArena {
  _PagedArenaPage *head, *tail;
  size_t page_size;
};

/**
 * @brief Creates a new PagedArena object.
 *
 * This function creates a new PagedArena object and allocates memory for it.
 * The size of each page in the PagedArena is set to the size specified by the
 * parameter. The head and tail of the PagedArena are set to a new
 * _PagedArenaPage object.
 *
 * @param page_size The size of each page in the PagedArena.
 *
 * @return A pointer to the newly created PagedArena object.
 */
__CCMS__INLINE
PagedArena* paged_arena__new(const size_t page_size) {
  PagedArena* self = _M_new(PagedArena);

  self->page_size = page_size;
  self->head = self->tail = _paged_arena_page__new(page_size, NULL);

  return self;
}

/**
 * @brief Frees a PagedArena object.
 *
 * This function frees a PagedArena object and all its associated
 * _PagedArenaPage objects.
 *
 * @param self A pointer to the PagedArena object to be freed.
 */
__CCMS__INLINE
void paged_arena__free(PagedArena* self) {
  for (_PagedArenaPage *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _paged_arena_page__free(itr);
  }
  _M_free(self);
}

/**
 * @brief Resets a PagedArena object.
 *
 * This function resets a PagedArena object by resetting each of its
 * _PagedArenaPage objects. This effectively makes the entire PagedArena
 * unoccupied.
 *
 * @param self A pointer to the PagedArena object to be reset.
 */
__CCMS__INLINE
void paged_arena__reset(PagedArena* self) {
  for (_PagedArenaPage* itr = self->head; itr != NULL; itr = itr->next)
    _paged_arena_page__reset(itr);

  self->tail = self->head;
}

/**
 * @brief Performs a hard reset on a PagedArena object.
 *
 * This function performs a hard reset on a PagedArena object by freeing all
 * _PagedArenaPage objects except the head, resetting the head, and setting the
 * tail of the PagedArena to its head. This effectively makes the entire
 * PagedArena unoccupied and frees up memory used by all pages except the first
 * one.
 *
 * @param self A pointer to the PagedArena object to be hard reset.
 */
__CCMS__INLINE
void paged_arena__hard_reset(PagedArena* self) {
  if (self->head->next != NULL)
    for (_PagedArenaPage *itr = self->head->next, *tmp; itr != NULL;
         itr = tmp) {
      tmp = itr->next;
      _paged_arena_page__free(itr);
    }

  _paged_arena_page__reset(self->head);
  self->head->next = NULL;
  self->tail = self->head;
}

/**
 * @brief Allocates a chunk of memory from a PagedArena object.
 *
 * This function allocates a chunk of memory of a specified size from a
 * PagedArena object. If the size requested is larger than the page size of the
 * PagedArena, a warning is printed and NULL is returned. If there is not enough
 * space in the current page (_PagedArenaPage), a new page is allocated and
 * added to the PagedArena. The position in the current page is updated to
 * reflect the allocation.
 *
 * @param self A pointer to the PagedArena object from which to allocate memory.
 * @param size The size of the memory chunk to be allocated.
 *
 * @return A pointer to the allocated memory chunk, or NULL if the size
 * requested is larger than the page size of the PagedArena.
 */
__CCMS__INLINE
uint8_t* paged_arena__alloc(PagedArena* self, const size_t size) {
  // If the requested size is larger than the page size of the PagedArena
  if (size > self->page_size) {
#ifndef __CCMS__SUPPRESS_WARNINGS
    // Print a warning message
    fprintf(stderr,
            "warning: tried allocating a chunk of size %ld from an arena (page "
            "allocated) with page size %ld, returned NULL\n",
            size, self->page_size);
#endif
    // Return NULL as we cannot allocate a chunk larger than the page size
    return NULL;
  }

  // If the remaining space in the current page is less than the requested size
  if (self->page_size - self->tail->pos < size) {
    if (self->tail->next == NULL)
      // Allocate a new page and set it as the next page
      self->tail->next = _paged_arena_page__new(self->page_size, NULL);

    // Move tail to the next page
    self->tail = self->tail->next;
  }

  // Calculate the address of the new chunk by adding the size of the
  // _PagedArenaPage struct and the current position to the address of the
  // current page
  uint8_t* result =
      _M_cast(uint8_t*, self->tail) + sizeof(_PagedArenaPage) + self->tail->pos;
  // Update the position in the current page
  self->tail->pos += size;

  // Return the address of the new chunk
  return result;
}

/**
 * @brief Calculates the average utilization of a PagedArena object.
 *
 * This function calculates the average utilization of a PagedArena object by
 * summing the utilization of each _PagedArenaPage (current position divided by
 * page size) and dividing by the total number of pages.
 *
 * @param self A pointer to the PagedArena object for which to calculate the
 * average utilization.
 *
 * @return The average utilization of the PagedArena object.
 */
__CCMS__INLINE
float paged_arena__avg_util(const PagedArena* self) {
  float sum = 0.f;
  size_t len = 0;

  for (_PagedArenaPage* itr = self->head; itr != NULL; itr = itr->next, len++)
    sum += _M_cast(float, itr->pos) / self->page_size;

  return sum / len;
}

#ifdef __cplusplus
}
#endif

#endif  // __CCMS__ARENAS__PAGED__H
