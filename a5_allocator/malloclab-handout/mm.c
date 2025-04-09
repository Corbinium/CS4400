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

void packHeader(struct header *h, size_t sizeForward, size_t sizeReverse, char alloc) {
  h->sizeForward = sizeForward | (alloc & 0x1);
  h->sizeReverse = sizeReverse;
}

void allocate_new_page(size_t size);

void add_free(void *f);
void remove_free(void *f);

void check_implicit_list(void *p);
void check_implicit_cycle(void *p);
void check_explicit_list(void *f);
void check_explicit_cycle(void *f);

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
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size)
{
  int newsize = ALIGN(size);
  struct header *p;

  if (current_page == NULL) {
    allocate_new_page(newsize);
  }

  p = (struct header*)current_page;
  while (1) {
    while (!IS_TERMINATOR(p)) {
      if (!GET_ALLOC(p) && GET_SIZE(p) >= newsize) {
        if (GET_SIZE(p) - newsize > sizeof(struct header)) {
          size_t newSize1 = newsize + sizeof(struct header);
          size_t newSize2 = GET_SIZE(p) - newsize;

          struct header *next = GET_NEXT(p);
          struct header *newBlock = (void*)p + newSize1;

          packHeader(newBlock, newSize2, newSize1, 0);
          packHeader(next, next->sizeForward, newSize2, GET_ALLOC(next));
          packHeader(p, newSize1, p->sizeReverse, 1);

          #ifdef DEBUG
            check_implicit_cycle(GET_PAYLOAD(newBlock));
          #endif
        }
        packHeader(p, p->sizeForward, p->sizeReverse, 1);

        #ifdef DEBUG
          check_implicit_list(GET_PAYLOAD(p));
        #endif

        return GET_PAYLOAD(p);
      }

      p = GET_NEXT(p);
    }

    allocate_new_page(newsize);
    p = (struct header*)current_page;
  }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
  struct header *p = GET_HEADER(ptr);

  struct header *next = GET_NEXT(p);
  if (!IS_TERMINATOR(next) && !GET_ALLOC(next)) {
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(p) + GET_SIZE(next) + sizeof(struct header)*2;
    packHeader(p, newSize, p->sizeReverse, 0);
    packHeader(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    #ifdef DEBUG
      check_implicit_cycle(GET_PAYLOAD(p));
    #endif
  }

  if (!IS_SENTINEL(p) && !GET_ALLOC(GET_PREV(p))) {
    p = GET_PREV(p);
    struct header *next = GET_NEXT(p);
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(p) + GET_SIZE(next) + sizeof(struct header)*2;
    packHeader(p, newSize, p->sizeReverse, 0);
    packHeader(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    #ifdef DEBUG
      check_implicit_cycle(GET_PAYLOAD(p));
    #endif
  }

  #ifdef DEBUG
    check_implicit_list(GET_PAYLOAD(p));
  #endif

  if (IS_SENTINEL(p) && IS_TERMINATOR(GET_NEXT(p))) {
    if (p == current_page) {
      current_page = NULL;
      current_page_size = 0;
    }
    mem_unmap(p, PAGE_ALIGN(GET_SIZE(p) + sizeof(struct header)*2));
  }
}

/********************************************************
 * Helper Functions
 ********************************************************/
void allocate_new_page(size_t size) {
  size_t newsize = PAGE_ALIGN(size + sizeof(struct header)*2);
  void *p = mem_map(newsize);
  if (p == NULL) {
    return;
  }
  
  struct header *sentinal = (struct header *)p;
  packHeader(sentinal, newsize-sizeof(struct header), 0, 0);

  struct header *terminator = GET_NEXT(sentinal);
  packHeader(terminator, 0, newsize-sizeof(struct header), 0);
  
  current_page = p;
  current_page_size = newsize;
}

void add_free(void *f) {}

void remove_free(void *f) {}

/********************************************************
 * Heap Checkers
 ********************************************************/

void check_implicit_list(void *currentBlock) {
  struct header *p = GET_HEADER(currentBlock);
  struct header *start = p;

  void *prev = p;
  p = GET_NEXT(p);
  while (p->sizeForward != 0) {
    if ((p->sizeForward & ~0xf) % 16 != 0) {
      printf("Error: sizeForward is not aligned to 16 bytes\n\tp: %p, prev: %p\n", p, prev);
    }
    if (GET_PREV(p) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
    }
    if (!GET_ALLOC(prev) && !GET_ALLOC(p)) {
      printf("Error: two consecutive free blocks moving forward\n\tp: %p, prev: %p\n", p, prev);
    }
    prev = p;
    p = GET_NEXT(p);
  }
  if (GET_PREV(p) != prev) {
    printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
  }

  prev = p;
  p = GET_PREV(p);
  while (p->sizeReverse != 0) {
    if (p->sizeReverse % 16 != 0) {
      printf("Error: sizeReverse is not aligned to 16 bytes\n\tp: %p, prev: %p\n", p, prev);
    }
    if (GET_NEXT(p) != prev) {
      printf("Error: next pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
    }
    if (((struct header*)prev)->sizeForward != 0 && !GET_ALLOC(p) && !GET_ALLOC(prev)) {
      printf("Error: two consecutive free blocks moving backward\n\tp: %p, prev: %p\n", p, prev);
    }
    prev = p;
    p = GET_PREV(p);
  }
  if (GET_NEXT(p) != prev) {
    printf("Error: next pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
  }

  if (p == start) {
    return;
  }
  prev = p;
  p = GET_NEXT(p);
  while (p != start) {
    if (p->sizeForward == 0) {
      printf("Error: sizeForward is 0\n\tp: %p, prev: %p\n", p, prev);
      exit(1);
    }
    if ((p->sizeForward & ~0xf) % 16 != 0) {
      printf("Error: sizeForward is not aligned to 16 bytes\n\tp: %p, prev: %p\n", p, prev);
    }
    if (GET_PREV(p) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
    }
    if (!GET_ALLOC(prev) && !GET_ALLOC(p)) {
      printf("Error: two consecutive free blocks moving forward\n\tp: %p, prev: %p\n", p, prev);
    }
    prev = p;
    p = GET_NEXT(p);
  }
  if (GET_PREV(p) != prev) {
    printf("Error: previous pointer does not point to the correct block\n\tp: %p, prev: %p\n", p, prev);
  }
}

void check_implicit_cycle(void *currentBlock) {
  struct header *p = GET_HEADER(currentBlock);
  struct header *start = p;

  p = GET_PREV(GET_NEXT(p));
  if (p != start) {
    printf("Error: implicit list is not forward circular\n\tp: %p\n", p);
  }

  if (!IS_SENTINEL(p)) {
    p = GET_NEXT(GET_PREV(p));
    if (p != start) {
      printf("Error: implicit list is not backward circular\n\tp: %p\n", p);
    }
  }
}

void check_explicit_list(void *f) {}

void check_explicit_cycle(void *f) {}