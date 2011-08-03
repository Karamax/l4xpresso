/*
L4Xpresso
Copyright (c) 2011, Sergey Klyaus

File: /leo4-mcu/kernel/src/fifo.c
Author: myaut

@LICENSE
*/

#include <fifo.h>

uint32_t fifo_init(struct fifo_t* queue, uint8_t* addr, uint32_t size) {
	queue->q_top = 0;
	queue->q_end = 0;
	queue->q_size = size;
	queue->q_data = addr;

	return FIFO_OK;
}

uint32_t fifo_push(struct fifo_t* queue, uint8_t el) {
	if(fifo_length(queue) == queue->q_size) {
		return FIFO_OVERFLOW;
	}

	++queue->q_end;

	if(queue->q_end == (queue->q_size - 1))
		queue->q_end = 0;

	queue->q_data[queue->q_end] = el;

	return FIFO_OK;
}

uint32_t fifo_state(struct fifo_t* queue) {
	if(fifo_length(queue) == 0) {
		return FIFO_EMPTY;
	}

	return FIFO_OK;
}

uint32_t fifo_pop(struct fifo_t* queue, uint8_t* el) {
	if(fifo_length(queue) == 0) {
		return FIFO_EMPTY;
	}

	++queue->q_top;

	if(queue->q_top == (queue->q_size - 1))
		queue->q_top = 0;

	*el = queue->q_data[queue->q_top];

	return FIFO_OK;
}

/*Функция fifo_length возвращает длину очереди*/
uint32_t fifo_length(struct fifo_t* queue) {
	return (queue->q_end >= queue->q_top)
			? (queue->q_end - queue->q_top)
			: (queue->q_size + queue->q_top - queue->q_end);
}
