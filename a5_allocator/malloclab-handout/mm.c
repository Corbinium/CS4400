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

/* Predefined variable for heap checking, if set to any number other then 0 the heap checker will be used */
#define DEBUG 0

/* always use 16-byte alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/* rounds up to the nearest multiple of mem_pagesize() */
#define PAGE_ALIGN(size) (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))

/* 
 * Header structure for each block. 
 * The sizeForward field contains the amount of bytes to traverse from the current byte to the next header.
 * The sizeReverse field contains the amount of bytes to traverse from the current byte to the previous header.
 * The last 4 bits of sizeForward are used to indicate if the block is allocated or not.
 */
struct header {
  size_t sizeForward;
  size_t sizeReverse;
};

/* 
 * Free list node structure. 
 * The free list is a doubly linked list of free blocks.
 * The next and prev fields point to the next and previous free blocks in the list.
 */
struct free_node {
  struct free_node *next;
  struct free_node *prev;
};

/* Given a payload pointer get the header pointer */
#define GET_HEADER(p) ((struct header*)((void*)(p) - sizeof(struct header)))

/* Given a header pointer do the stated operation */
#define GET_SIZE(h) ((size_t)((void*)(((struct header*)(h))->sizeForward & ~0xF) - sizeof(struct header)))
#define GET_NEXT(h) ((void*)(h) + (((struct header*)(h))->sizeForward & ~0xF))
#define GET_PREV(h) ((void*)(h) - ((struct header*)(h))->sizeReverse) 
#define GET_ALLOC(h) (((struct header*)(h))->sizeForward & 0x1)
#define SET_ALLOC(h, alloc) (((struct header*)(h))->sizeForward = (((struct header*)h)->sizeForward & ~0xF) | (alloc & 0x1))
#define GET_PAYLOAD(h) ((void*)((void*)(h) + sizeof(struct header)))
#define IS_TERMINATOR(h) (((struct header*)(h))->sizeForward == 0)
#define IS_SENTINEL(h) (((struct header*)(h))->sizeReverse == 0)

/* Given a free_node pointer do the stated operation */
#define GET_PREV_FREE(f) (((struct free_node*)(f))->prev)
#define GET_NEXT_FREE(f) (((struct free_node*)(f))->next)
#define IS_FIRST_FREE(f) (((struct free_node*)(f))->prev == NULL)
#define IS_LAST_FREE(f) (((struct free_node*)(f))->next == NULL)

/* Helper Functions */
void pack_header(struct header *h, size_t sizeForward, size_t sizeReverse, char alloc);
void allocate_new_page(size_t size);
char seperate_page(struct header *h, size_t size);
void pack_free(struct free_node *f, struct free_node *prev, struct free_node *next);
void add_free(void *f);
void remove_free(void *f);

/* Checker Functions */
void check_implicit_list(void *p);
void check_implicit_cycle(void *p);
void check_explicit_list();
void check_explicit_cycle(void *f);

/* Global Variables */
void *first_free = NULL;

/**********************************************************
 * Primary Functions
 **********************************************************/

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  first_free = NULL;
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block of memory of "size" bytes. 
 * If the desired size is not a multiple of 16 it will be rounded up.
 * If the desired size is larger than the largest available space in the heap a new page will be allocated.
 * Otherwise it will search the free list for a block that is large enough to fit the requested size and allocate it.
 */
void *mm_malloc(size_t size)
{
  int newsize = ALIGN(size);
  struct header *h;

  // If this is the first call to malloc, allocate a new page
  if (first_free == NULL) {
    allocate_new_page(newsize);
  }

  // Search for a free block that is large enough to fit the requested size
  struct free_node *f = first_free;
  while (f != NULL) {
    h = GET_HEADER(f);
    if (seperate_page(h, newsize)) { return GET_PAYLOAD(h); }
    f = GET_NEXT_FREE(f);
  }

  // If no free block is found, allocate a new page
  allocate_new_page(newsize);
  h = GET_HEADER(first_free);
  if (seperate_page(h, newsize)) { return GET_PAYLOAD(h); }
  printf("Error: malloc failed to allocate memory\n");
  exit(1);
}

/*
 * mm_free - Free the block of allocated memory at pointer p.
 * The block is marked as free and added to the free list.
 * If the next block is also free, it is coalesced with the current block.
 * If the previous block is also free, it is coalesced with the current block.
 * If the current block is a sentinel and the next block is a terminator, the page is unallocated.
 * However if the current block is also the last free block, it is not unallocated.
 */
void mm_free(void *p)
{
  // Collect the header and add the current block to the explicit free list
  struct header *h = GET_HEADER(p);
  SET_ALLOC(h, 0);
  add_free(p);

  // If the next block is free, coalesce with it
  struct header *next = GET_NEXT(h);
  if (!IS_TERMINATOR(next) && !GET_ALLOC(next)) {
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(h) + GET_SIZE(next) + sizeof(struct header)*2;
    pack_header(h, newSize, h->sizeReverse, 0);
    pack_header(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    remove_free(GET_PAYLOAD(next));
    #if DEBUG
      check_implicit_cycle(GET_PAYLOAD(h));
      check_explicit_cycle(GET_PAYLOAD(h));
    #endif
  }

  // If the previous block is free, coalesce with it
  if (!IS_SENTINEL(h) && !GET_ALLOC(GET_PREV(h))) {
    h = GET_PREV(h);
    struct header *next = GET_NEXT(h);
    struct header *nextNext = GET_NEXT(next);
    size_t newSize = GET_SIZE(h) + GET_SIZE(next) + sizeof(struct header)*2;
    pack_header(h, newSize, h->sizeReverse, 0);
    pack_header(nextNext, nextNext->sizeForward, newSize, GET_ALLOC(nextNext));
    remove_free(GET_PAYLOAD(next));
    #if DEBUG
      check_implicit_cycle(GET_PAYLOAD(h));
      check_explicit_cycle(GET_PAYLOAD(h));
    #endif
  }

  #if DEBUG
    check_implicit_list(GET_PAYLOAD(h));
    check_explicit_list();
  #endif

  // If the current block is a sentinel and the next block is a terminator, unallocate the page
  // If the current block is the last free block, do not unallocate the page
  if (IS_SENTINEL(h) && IS_TERMINATOR(GET_NEXT(h)) && !IS_LAST_FREE(GET_PAYLOAD(h))) {
    remove_free(GET_PAYLOAD(h));
    mem_unmap(h, PAGE_ALIGN(GET_SIZE(h) + sizeof(struct header)*2));
  }
}

/********************************************************
 * Helper Functions
 ********************************************************/

/*
 * pack_header - Pack the header with the given size and allocation status.
 */
void pack_header(struct header *h, size_t sizeForward, size_t sizeReverse, char alloc) {
  h->sizeForward = sizeForward | alloc;
  h->sizeReverse = sizeReverse;
}

/*
 * allocate_new_page - Allocate a new page of memory.
 * The new page will be sized such that the current size of the heap is doubled.
 * However if the this size is more then 32 pages, the size will be capped at 32 pages.
 * The new page will be added to the free list and sentinal/terminator headers will be added.
 */
void allocate_new_page(size_t size) {
  size_t newsize = size + sizeof(struct header)*2;
  void *p;

  // Get the current size of the heap and double it
  size_t currentSize = mem_heapsize();
  size_t desiredSize;
  size_t maxSize = PAGE_ALIGN(32*4096);
  if (currentSize == 0) {
    desiredSize = mem_pagesize();
  }
  else {
    desiredSize = PAGE_ALIGN(currentSize*2);
  }

  // If the requested size is larger then what would be available by doubling the current size of the heap
  // continue to double the size until it is sufficently large
  while (desiredSize-currentSize < newsize) {
    desiredSize += desiredSize;
  }

  // If the desired size is larger then 32 pages, cap it at 32 pages
  if (desiredSize-currentSize >= maxSize) {
    newsize = maxSize;
  }
  else {
    newsize = PAGE_ALIGN(desiredSize-currentSize);
  }

  // Allocate the new page
  p = mem_map(newsize);
  if (p == NULL) {
    exit(1);
  }
  
  // Setup the sentinal and terminator headers, and add the new page to the free list
  struct header *sentinal = (struct header *)p;
  pack_header(sentinal, newsize-sizeof(struct header), 0, 0);
  add_free(GET_PAYLOAD(p));
  struct header *terminator = GET_NEXT(sentinal);
  pack_header(terminator, 0, newsize-sizeof(struct header), 0);
}

/*
 * seperate_page - check if the current free block is large enough to fit the requested size.
 * If the free block is large enough to fit the requested size and another block it will be split into two blocks.
 * The first block will be allocated and the second block will be added to the free list.
 * If the block is not large enough to fit the requested size the function will return 0.
 */
char seperate_page(struct header *h, size_t size) {
  // If the block is large enough to fit the requested size
  if (GET_SIZE(h) >= size) {
    // If the block is large enough to fit the requested size and another block
    if (GET_SIZE(h) - size > sizeof(struct header)) {
      // Calculate the size of the allocated block and the new block after it
      size_t newSize1 = size + sizeof(struct header);
      size_t newSize2 = GET_SIZE(h) - size;

      // Locate needed header pointers
      struct header *next = GET_NEXT(h);
      struct header *newBlock = (void*)h + newSize1;
      
      // Pack the headers and add the new block to the free list
      pack_header(newBlock, newSize2, newSize1, 0);
      add_free(GET_PAYLOAD(newBlock));
      pack_header(next, next->sizeForward, newSize2, GET_ALLOC(next));
      pack_header(h, newSize1, h->sizeReverse, 1);

      #if DEBUG
        check_implicit_cycle(GET_PAYLOAD(newBlock));
      #endif
    }
    // Allocate the block and remove it from the free list
    SET_ALLOC(h, 1);
    remove_free(GET_PAYLOAD(h));

    #if DEBUG
      check_implicit_list(GET_PAYLOAD(h));
      check_explicit_list();
    #endif

    return 1;
  }

  return 0;
}

/*
 * pack_free - Pack the free node with the given previous and next pointers.
 */
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

/*
 * add_free - Add a free block to the free list.
 * If the free list is empty, the block is added as the first free block.
 * If the free list is not empty, the block is added to the front of the free list.
 */
void add_free(void *f) {
  // If the free list is empty, add the block as the first free block
  if (first_free == NULL) {
    pack_free(f, NULL, NULL);
    first_free = f;
  }
  // If the free list is not empty, add the block to the front of the free list
  else {
    struct free_node *next = first_free;
    pack_free(next, f, next->next);
    pack_free(f, NULL, next);
    first_free = f;
  }
}

/*
 * remove_free - Remove a free block from the free list.
 * If the block is the first free block, the first free block pointer is updated to the next block.
 */
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

 /*
  * check_implicit_list - Check the implicit list for errors.
  * This function checks if the size is misaligned and if the previous and next pointers are correct.
  * It also checks if there are two consecutive free blocks moving forward or backward.
  * It checks this by iterativly progressing through the list from block p to the end of the list.
  * It will then iterativly progress through the list from the end to the beginning.
  * Finally it will iterativly progress through the list from the beginning to block p.
  */
void check_implicit_list(void *p) {
  struct header *h = GET_HEADER(p);
  struct header *start = h;

  // Progress from p to the end of the list
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

  // Progress from the end of the list to the beginning
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

  // Progress from the beginning of the list to block p
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

/*
 * check_implicit_cycle - Check the implicit list for circularity.
 * This function checks if the previous pointer of the next block is the same as the current block.
 * It also checks if the next pointer of the previous block is the same as the current block.
 */
void check_implicit_cycle(void *p) {
  struct header *h = GET_HEADER(p);
  struct header *start = h;

  // Check the previous pointer of the next block
  h = GET_PREV(GET_NEXT(h));
  if (h != start) {
    printf("Error: implicit list is not forward circular\n\tp: %p\n", h);
  }

  // Check the next pointer of the previous block
  if (!IS_SENTINEL(h)) {
    h = GET_NEXT(GET_PREV(h));
    if (h != start) {
      printf("Error: implicit list is not backward circular\n\tp: %p\n", h);
    }
  }
}

/*
 * check_explicit_list - Check the explicit free list for errors.
 * This function checks if the next and previous pointers are correct.
 * It does this by iterativly progressing from the start of the list to the end of the list.
 * It then progresses iterativly from the end of the list to the beginning.
 */
void check_explicit_list() {
  if (first_free == NULL) {
    return;
  }

  // Progress from the start of the list to the end of the list
  struct free_node *prev = (struct free_node*)first_free;
  struct free_node *next = GET_NEXT_FREE(prev);
  while (next != NULL) {
    if (GET_PREV_FREE(next) != prev) {
      printf("Error: previous pointer does not point to the correct block\n\tnext: %p, prev: %p\n", next, prev);
    }
    prev = next;
    next = GET_NEXT_FREE(next);
  }

  // Progress from the end of the list to the beginning
  next = GET_PREV_FREE(prev);
  while (next != NULL) {
    if (GET_NEXT_FREE(next) != prev) {
      printf("Error: next pointer does not point to the correct block\n\tnext: %p, prev: %p\n", next, prev);
    }
    prev = next;
    next = GET_PREV_FREE(next);
  }
}

/*
 * check_explicit_cycle - Check the explicit free list for circularity.
 * This function checks if the next pointer of the previous block is the same as the current block.
 * It also checks if the previous pointer of the next block is the same as the current block.
 */
void check_explicit_cycle(void *f) {
  struct free_node *start = (struct free_node*)f;

  // Check the previous pointer of the next block
  if (!IS_LAST_FREE(f)) {
    f = GET_PREV_FREE(GET_NEXT_FREE(f));
    if (f != start) {
      printf("Error: explicit list is not forward circular\n\tf: %p\n", f);
    }
  }
  
  // Check the next pointer of the previous block
  if (!IS_FIRST_FREE(f)) {
    f = GET_NEXT_FREE(GET_PREV_FREE(f));
    if (f != start) {
      printf("Error: explicit list is not backward circular\n\tf: %p\n", f);
    }
  }
}