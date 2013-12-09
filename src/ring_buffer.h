#ifndef RINGBUFFER_H
#define RINGBUGGER_H

#include "types.h"
#include "kheap.h"

typedef struct ring_buffer {
  uint32_t head;
  uint32_t tail;
  uint32_t count; // How many items are currently in the buffer
  uint32_t size; // TODO: can we really have a buffer this large?
  uint8_t * data; // our data buffer
} ring_buffer;

ring_buffer * ring_buffer_create(size_t size);
uint8_t ring_buffer_read(ring_buffer *buffer);
void ring_buffer_write(ring_buffer *buffer, uint8_t item);

#endif
