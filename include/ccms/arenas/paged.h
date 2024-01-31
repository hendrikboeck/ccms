#ifndef __CCMS__ARENAS__PAGED__H
#define __CCMS__ARENAS__PAGED__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#ifndef __CCMS_SUPPRESS_WARNINGS
#include <stdio.h>
#endif

#include "ccms/_macros.h"

typedef struct _PagedArenaPage _PagedArenaPage;
typedef struct PagedArena PagedArena;

struct _PagedArenaPage {
  _PagedArenaPage *next;
  size_t pos;
};

static inline _PagedArenaPage *_paged_arena_page__new(const size_t size,
                                                      _PagedArenaPage *next) {
  _PagedArenaPage *self =
      _M_cast(_PagedArenaPage *, _M_alloc(sizeof(_PagedArenaPage) + size));

  self->pos = 0;
  self->next = next;

  return self;
}

static inline void _paged_arena_page__free(_PagedArenaPage *self) {
  _M_free(self);
}

static inline void _paged_arena_page__reset(_PagedArenaPage *self) {
  self->pos = 0;
}

struct PagedArena {
  _PagedArenaPage *head, *tail;
  size_t page_size;
};

static inline PagedArena *paged_arena__new(const size_t page_size) {
  PagedArena *self = _M_new(PagedArena);

  self->page_size = page_size;
  self->head = self->tail = _paged_arena_page__new(page_size, NULL);

  return self;
}

static inline void paged_arena__free(PagedArena *self) {
  for (_PagedArenaPage *itr = self->head, *tmp; itr != NULL; itr = tmp) {
    tmp = itr->next;
    _paged_arena_page__free(itr);
  }
  _M_free(self);
}

static inline void paged_arena__reset(PagedArena *self) {
  for (_PagedArenaPage *itr = self->head; itr != NULL; itr = itr->next)
    _paged_arena_page__reset(itr);

  self->tail = self->head;
}

static inline void paged_arena__hard_reset(PagedArena *self) {
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

static inline uint8_t *paged_arena__alloc(PagedArena *self, const size_t size) {
  if (size > self->page_size) {
#ifndef __CCMS_SUPPRESS_WARNINGS
    fprintf(stderr,
            "warning: tried allocating a chunk of size %ld from an arena (page "
            "allocated) with page size %ld, returned NULL\n",
            size, self->page_size);
#endif
    return NULL;
  }

  if (self->page_size - self->tail->pos < size) {
    if (self->tail->next == NULL)
      self->tail->next = _paged_arena_page__new(self->page_size, NULL);

    self->tail = self->tail->next;
  }

  uint8_t *result = _M_cast(uint8_t *, self->tail) + sizeof(_PagedArenaPage) +
                    self->tail->pos;
  self->tail->pos += size;

  return result;
}

static inline float paged_arena__avg_util(const PagedArena *self) {
  float sum = 0.f;
  size_t len = 0;

  for (_PagedArenaPage *itr = self->head; itr != NULL; itr = itr->next, len++)
    sum += _M_cast(float, itr->pos) / self->page_size;

  return sum / len;
}

#ifdef __cplusplus
}
#endif

#endif // __CCMS__ARENAS__PAGED__H
