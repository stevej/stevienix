// kheap.h -- Interface for kernel heap functions, also provides
//            a placement malloc() for use before the heap is
//            initialised.
//            Written for JamesM's kernel development tutorials.

#ifndef _KHEAP_H
#define _KHEAP_H

#include "ordered_array.h"
#include "paging.h"
#include "types.h"

#define KHEAP_START 0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000

#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x70000
#define HEAP_PAGE_SIZE 0x1000

/**
 *  Size information for a hole/block
 */
typedef struct {
  u32 magic;  // Magic number, used for error checking and identification.
  u8 is_hole; // 1 if this is a hole. 0 if this is a block.
  u32 size;   // size of the block, including the end footer.
} header_t;

typedef struct {
  u32 magic;        // Magic number, same as in header_t.
  header_t *header; // Pointer to the block header.
} footer_t;

typedef struct {
  ordered_array_t index;
  u32 start_address; // The start of our allocated space.
  u32 end_address;   // The end of our allocated space. May be expanded up to
                     // max_address.
  u32 max_address;   // The maximum address the heap can be expanded to.
  u8 supervisor;     // Should extra pages requested by us be mapped as
                     // supervisor-only?
  u8 readonly; // Should extra pages requested by us be mapped as read-only?
} heap_t;

typedef struct hole_search_results {
  union {
    union {
      i32 notfound;
      u32 found_at_location;
    };
    i32 results_found; // If this is greater than -1 then found_at_location will
                       // have what you're looking for.
  };                   // if this is negative then no results found.
} hole_search_results;

/**
 * Create a new heap.
 */
heap_t *create_heap(u32 start, u32 end, u32 max, u8 supervisor, u8 readonly);

/**
 * Allocates a contiguous region of memory 'size' in size. If page_align==1, it
 * creates that block starting on a page boundary.
 */
void *alloc(u32 size, u8 page_align, heap_t *heap);

/**
 * Releases a block allocated with 'alloc'.
 */
void free(void *p, heap_t *heap);

/**
 * Allocate a chunk of memory, sz in size. If align == 1,
 * the chunk must be page-aligned. If phys != 0, the physical
 * location of the allocated chunk will be stored into phys.
 *
 * This is the internal version of kmalloc. More user-friendly
 * parameter representations are available in kmalloc, kmalloc_a,
 * kmalloc_ap, kmalloc_p.
 */
u32 kmalloc_int(u32 sz, int align, u32 *phys);

/**
 * Allocate a chunk of memory, sz in size. The chunk must be
 * page aligned.
 */
u32 kmalloc_a(u32 sz);

/**
 * Allocate a chunk of memory, sz in size. The physical address
 * is returned in phys. Phys MUST be a valid pointer to u32!
 */
u32 kmalloc_p(u32 sz, u32 *phys);

/**
 * Allocate a chunk of memory, sz in size. The physical address
 * is returned in phys. It must be page-aligned.
 */
u32 kmalloc_ap(u32 sz, u32 *phys);

/**
 * General allocation function.
 */
u32 kmalloc(u32 sz);

/**
 * General deallocation function.
 */
void kfree(void *p);

/**
 * Setting our placement to a new address
 */
void kheap_set_placement_address(u32 end);

#endif // _KHEAP_H
