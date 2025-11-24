#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include<stdint.h>
#include<stdbool.h>

#define RING_BUFFER_SIZE 32

typedef struct {
	volatile uint8_t buffer[RING_BUFFER_SIZE];
	volatile uint16_t head; //次に書き込む位置
	volatile uint16_t tail; //読み込む位置
} RingBuffer;

void rb_init(RingBuffer *rb);
bool rb_is_empty(RingBuffer *rb);
bool rb_is_full(RingBuffer *rb);
bool rb_push(RingBuffer *rb, uint8_t data);
bool rb_pop(RingBuffer *rb, uint8_t *data);
//void print_ring_buff(RingBuffer *rb);

#endif // RING_BUFFER_H
