#include "ring_buffer.h"

/**
 * WARNING: this ring buffer currently works if the consumer keeps up
 * with the producer but otherwise suffers from overflow and underflow
 * issues. (tail can overwrite head and head can read past the
 * end. that will be fixed when I implement semaphores and wait())
 */

// For optimum performance, choose a power of two.
ring_buffer * ring_buffer_create(size_t size) {
  ring_buffer *buffer;
  buffer = (ring_buffer *)kmalloc(sizeof(*buffer));
  buffer->size = size;
  buffer->count = 0;
  buffer->head = 0; // read location
  buffer->tail = 0; // write location
  buffer->data = (uint8_t *)kmalloc(size * sizeof(uint8_t));

  return buffer;
}

/**
 * returns the item on the head of the buffer, removes the item.
 */
uint8_t ring_buffer_read(ring_buffer *buffer) {
  // TODO: synchronize this whole block around a semaphore. head must not change.
  // If head is at tail, then block until a write occurs?
  uint32_t old_head = buffer->head;
  uint32_t item = buffer->data[old_head];
  uint32_t new_head = (old_head + 1) % buffer->size;
  buffer->head = new_head;
  buffer->count--;

  return item;
}

// writes to the tail of the buffer.
void ring_buffer_write(ring_buffer *buffer, uint8_t item) {
  // TODO: synchronize this whole block around a semaphore. tail must not change.
  // If tail is at head then block until a read occurs?

  if (buffer->count < buffer->size) {
    uint32_t new_tail = (buffer->tail + 1) % buffer->size;
    buffer->data[buffer->tail] = item;
    buffer->tail = new_tail;
    buffer->count++;
    printk("buffer count: %d, size: %d, head: %d, tail: %d\n",
           buffer->count, buffer->size, buffer->head, buffer->tail);
  } else {
    // We are full
    printk("write to ring_buffer %x dropped: won't overwrite head.\n", buffer);
  }
}
