/*
 * Corbin Barney
 * u1066089
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define DEBUG 1

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

/* Structures */
struct header {
  size_t sizeForward;
  size_t sizeReverse;
};
struct free_node {
  struct free_node *next;
  struct free_node *prev;
};

/* Given a payload pointer get the header pointer */
#define GET_HEADER(p) ((struct header*)((void*)(p) - sizeof(struct header)))

/* Given a header pointer do x */
#define GET_SIZE(h) ((size_t)((void*)(((struct header*)h)->sizeForward & ~0xF) - sizeof(struct header)))
#define GET_NEXT(h) ((void*)h + (((struct header*)h)->sizeForward & ~0xF))
#define GET_PREV(h) ((void*)h - ((struct header*)h)->sizeReverse) 
#define GET_ALLOC(h) (((struct header*)h)->sizeForward & 0x1)
#define SET_ALLOC(h, alloc) (((struct header*)h)->sizeForward = ((struct header*)h)->sizeForward | (alloc & 0x1))
#define GET_PAYLOAD(h) ((void*)((void*)h + sizeof(struct header)))
#define IS_TERMINATOR(h) (((struct header*)h)->sizeForward == 0)
#define IS_SENTINEL(h) (((struct header*)h)->sizeReverse == 0)

/* Operations on explicit free list */
#define GET_PREV_FREE(f) (((struct free_node*)f)->prev)
#define GET_NEXT_FREE(f) (((struct free_node*)f)->next)
#define IS_FIRST_FREE(f) (((struct free_node*)f)->prev == NULL)
#define IS_LAST_FREE(f) (((struct free_node*)f)->next == NULL)

/* Helper Functions */
void pack_header(struct header *h, size_t sizeForward, size_t sizeReverse, char alloc);
void allocate_new_page(size_t size);
void pack_free(struct free_node *f, struct free_node *prev, struct free_node *next);
void add_free(void *f);
void remove_free(void *f);

/* Checker Functions */
void check_implicit_list(void *p);
void check_implicit_cycle(void *p);
void check_explicit_list(void *f);
void check_explicit_cycle(void *f);

/* Global Variables */
void *current_page = NULL;
size_t current_page_size = 0;
void *first_free = NULL;

/**********************************************************
 * Primary Functions
 **********************************************************/

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  current_page = NULL;
  current_page_size = 0;
  first_free = NULL;
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  int newsize = ALIGN(size);
  struct header *h;

  if (current_page == NULL) {
    allocate_new_page(newsize);
  }

  h = (struct header*)current_page;
  while (1) {
    while (!IS_TERMINATOR(h)) {
      if (!GET_ALLOC(h) && GET_SIZE(h) >= newsize) {
        if (GET_SIZE(h) - newsize > sizeof(struct header)) {
          size_t newSize1 = newsize + sizeof(struct header);
          size_t newSize2 = GET_SIZE(h) - newsize;

          struct header *next = GET_NEXT(h);
          struct header *newBlock = (void*)h + newSize1;

          pack_header(newBlock, newSize2, newSize1, 0);
          pack_header(next, next->sizeForward, newSize2, GET_ALLOC(next));
          pack_header(h, newSize1, h->sizeReverse, 1);

          #ifdef DEBUG
            check_implicit_cycle(GET_PAYLOAD(newBlock));
          #endif
        }
        pack_header(h, h->sizeForward, h->sizeReverse, 1);

        #ifdef DEBUG
          check_implicit_list(GET_PAYLOAD(h));
        #endif

        return GET_PAYLOAD(h);
      }

      h = GET_NEXT(h);
    }

    allocate_new_page(newsize);
    h = (struct header*)current_page;
  }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *p)
{
  struct header *h = GET_HEADER(p);

  struct header *next = GET_NEXT(h);
  if (!IS_TERMINATOR(next) && !GET_ALLOC(next)) {
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(h) + GET_SIZE(next) + sizeof(struct header)*2;
    pack_header(h, newSize, h->sizeReverse, 0);
    pack_header(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    #ifdef DEBUG
      check_implicit_cycle(GET_PAYLOAD(h));
    #endif
  }

  if (!IS_SENTINEL(h) && !GET_ALLOC(GET_PREV(h))) {
    h = GET_PREV(h);
    struct header *next = GET_NEXT(h);
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(h) + GET_SIZE(next) + sizeof(struct header)*2;
    pack_header(h, newSize, h->sizeReverse, 0);
    pack_header(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    #ifdef DEBUG
      check_implicit_cycle(GET_PAYLOAD(h));
    #endif
  }

  #ifdef DEBUG
    check_implicit_list(GET_PAYLOAD(h));
  #endif

  if (IS_SENTINEL(h) && IS_TERMINATOR(GET_NEXT(h))) {
    if (h == current_page) {
      current_page = NULL;
      current_page_size = 0;
    }
    mem_unmap(h, PAGE_ALIGN(GET_SIZE(h) + sizeof(struct header)*2));
  }
}

/********************************************************
 * Helper Functions
 ********************************************************/
void pack_header(struct header *h, size_t sizeForward, size_t sizeReverse, char alloc) {
  h->sizeForward = sizeForward | alloc;
  h->sizeReverse = sizeReverse;

  // if (!alloc) {
  //   add_free(GET_PAYLOAD(h));
  // }
}

void allocate_new_page(size_t size) {
  size_t newsize = PAGE_ALIGN(size + sizeof(struct header)*2);
  void *p = mem_map(newsize);
  if (p == NULL) {
    return;
  }
  
  struct header *sentinal = (struct header *)p;
  pack_header(sentinal, newsize-sizeof(struct header), 0, 0);

  struct header *terminator = GET_NEXT(sentinal);
  pack_header(terminator, 0, newsize-sizeof(struct header), 0);
  
  current_page = p;
  current_page_size = newsize;
}

void pack_free(struct free_node *f, struct free_node *prev, struct free_node *next) {
  f->prev = prev;
  f->next = next;
  if (prev != NULL) {
    prev->next = f;
  }
  if (next != NULL) {
    next->prev = f;
  }
}

void add_free(void *f) {
  if (first_free == NULL) {
    pack_free(f, NULL, NULL);
    first_free = f;
  }
  else {
    struct free_node *next = first_free;
    pack_free(f, NULL, next);
    first_free = f;
  }
}

void remove_free(void *f) {
  struct free_node *prev = GET_PREV_FREE(f);
  struct free_node *next = GET_NEXT_FREE(f);
  if (prev != NULL) {
    prev->next = next;
  }
  if (next != NULL) {
    next->prev = prev;
  }
  if (first_free == f) {
    first_free = next;
  }
}

/********************************************************
 * Heap Checkers
 ********************************************************/

void check_implicit_list(void *p) {
  struct header *h = GET_HEADER(p);
  struct header *start = h;

  void *prev = h;
  h = GET_NEXT(h);
  while (h->sizeForward != 0) {
    if ((h->sizeForward & ~0xf) % 16 != 0) {
      printf("Error: sizeForward is not aligned to 16 bytes\n\tp: %p, prev: %p\n", h, prev);
    }
    if (GET_PREV(h) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
    }
    if (!GET_ALLOC(prev) && !GET_ALLOC(h)) {
      printf("Error: two consecutive free blocks moving forward\n\tp: %p, prev: %p\n", h, prev);
    }
    prev = h;
    h = GET_NEXT(h);
  }
  if (GET_PREV(h) != prev) {
    printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
  }

  prev = h;
  h = GET_PREV(h);
  while (h->sizeReverse != 0) {
    if (h->sizeReverse % 16 != 0) {
      printf("Error: sizeReverse is not aligned to 16 bytes\n\tp: %p, prev: %p\n", h, prev);
    }
    if (GET_NEXT(h) != prev) {
      printf("Error: next pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
    }
    if (((struct header*)prev)->sizeForward != 0 && !GET_ALLOC(h) && !GET_ALLOC(prev)) {
      printf("Error: two consecutive free blocks moving backward\n\tp: %p, prev: %p\n", h, prev);
    }
    prev = h;
    h = GET_PREV(h);
  }
  if (GET_NEXT(h) != prev) {
    printf("Error: next pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
  }

  if (h == start) {
    return;
  }
  prev = h;
  h = GET_NEXT(h);
  while (h != start) {
    if (h->sizeForward == 0) {
      printf("Error: sizeForward is 0\n\tp: %p, prev: %p\n", h, prev);
      exit(1);
    }
    if ((h->sizeForward & ~0xf) % 16 != 0) {
      printf("Error: sizeForward is not aligned to 16 bytes\n\tp: %p, prev: %p\n", h, prev);
    }
    if (GET_PREV(h) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
    }
    if (!GET_ALLOC(prev) && !GET_ALLOC(h)) {
      printf("Error: two consecutive free blocks moving forward\n\tp: %p, prev: %p\n", h, prev);
    }
    prev = h;
    h = GET_NEXT(h);
  }
  if (GET_PREV(h) != prev) {
    printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", h, prev);
  }
}

void check_implicit_cycle(void *p) {
  struct header *h = GET_HEADER(p);
  struct header *start = h;

  h = GET_PREV(GET_NEXT(h));
  if (h != start) {
    printf("Error: implicit list is not forward circular\n\tp: %p\n", h);
  }

  if (!IS_SENTINEL(h)) {
    h = GET_NEXT(GET_PREV(h));
    if (h != start) {
      printf("Error: implicit list is not backward circular\n\tp: %p\n", h);
    }
  }
}

void check_explicit_list(void *f) {
  struct free_node *start = (struct free_node*)f;

  struct free_node *prev = start;
  struct free_node *next = GET_NEXT_FREE(start);
  while (next != NULL) {
    if (GET_PREV_FREE(next) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tnext: %p, prev: %p\n", next, prev);
    }
    prev = next;
    next = GET_NEXT_FREE(next);
  }

  next = GET_PREV_FREE(prev);
  while (next != NULL) {
    if (GET_NEXT_FREE(next) != prev) {
      printf("Error: next pointer does not point to the correct block\n\tnext: %p, prev: %p\n", next, prev);
    }
    prev = next;
    next = GET_PREV_FREE(next);
  }

  next = GET_NEXT_FREE(prev);
  while (prev != start) {
    if (GET_PREV_FREE(next) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tnext: %p, prev: %p\n", next, prev);
    }
    prev = next;
    next = GET_NEXT_FREE(next);
  }
}

void check_explicit_cycle(void *f) {
  struct free_node *start = (struct free_node*)f;

  if (!IS_LAST_FREE(f)) {
    f = GET_PREV_FREE(GET_NEXT_FREE(f));
    if (f != start) {
      printf("Error: explicit list is not forward circular\n\tf: %p\n", f);
    }
  }
  
  if (!IS_FIRST_FREE(f)) {
    f = GET_NEXT_FREE(GET_PREV_FREE(f));
    if (f != start) {
      printf("Error: explicit list is not backward circular\n\tf: %p\n", f);
    }
  }
}