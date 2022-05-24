// paging.h -- Defines the interface for and structures relating to paging.
//             Written for JamesM's kernel development tutorials.

#ifndef _PAGING_H
#define _PAGING_H

#include "io.h"
#include "isr.h"
#include "screen.h"
#include "types.h"

// FIXME: derive proper types from process.s
extern void copy_page_physical(uint32_t a, uint32_t b);

typedef struct page {
  u32 present : 1;  // Page present in memory
  u32 rw : 1;       // Read-only if clear, readwrite if set
  u32 user : 1;     // Supervisor level only if clear
  u32 accessed : 1; // Has the page been accessed since last refresh?
  u32 dirty : 1;    // Has the page been written to since last refresh?
  u32 unused : 7;   // Amalgamation of unused and reserved bits
  u32 frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

extern void alloc_frame(page_t *, int, int);
void free_frame(page_t *page);

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  /**
   * Array of pointers to pagetables.
   */
  page_table_t *tables[1024];
  /**
   * Array of pointers to the pagetables above, but gives their *physical*
   * location, for loading into the CR3 register.
   */
  u32 tablesPhysical[1024];

  /**
   * The physical address of tablesPhysical. This comes into play
   * when we get our kernel heap allocated and the directory
   * may be in a different location in virtual memory.
   */
  u32 physicalAddr;
} page_directory_t;

/**
 * Sets up the environment, page directories etc and
 * enables paging.
 */
void initialise_paging();

/**
 * Causes the specified page directory to be loaded into the
 * CR3 register.
 */
void switch_page_directory(page_directory_t *new);

/**
 * Retrieves a pointer to the page required.
 * If make == 1, if the page-table in which this page should
 * reside isn't created, create it!
 */
page_t *get_page(u32 address, int make, page_directory_t *dir);

/**
 * Handler for page faults.
 */
void page_fault(registers_t *regs);

/**
 * Makes a copy of a page directory.
 */
page_directory_t *clone_directory(page_directory_t *src);

#endif // _PAGING_H
