#include "ring_bffer.h"
#include <stdio.h>
/* -------------------- */
void rb_init(RingBuffer *rb){
	rb->head = 0;
	rb->tail = 0;
}
/* -------------------- */
bool rb_is_empty(RingBuffer *rb){
	return rb->head == rb->tail;
}
/* -------------------- */
bool rb_is_full(RingBuffer *rb){
	return (rb->head + 1) % RING_BUFFER_SIZE == rb->tail;
}
bool rb_push(RingBuffer *rb, uint8_t data){
	if(rb_is_full(rb)) return false;
	rb->buffer[rb->head] = data;
	rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
	return true;
}
bool rb_pop(RingBuffer *rb, uint8_t *data){
	if(rb_is_empty(rb)) return false;
	*data = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
	return true;
}
