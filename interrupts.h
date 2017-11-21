#ifndef __INT_H
#define __INT_H
#include <gpio.h>
#include <stm32.h>
#include <stdbool.h>

#define INT_MAX_HANDLERS 10

/**
 * Automatyczne wspieranie przerwań.
 */

typedef enum {
	INT_DMA1S6		=	0,
	INT_EXTI0		=	1,
	INT_EXTI3		=	2,
	INT_EXTI4		=	3,
	INT_EXTI9_5		=	4,
	INT_EXTI15_10	=	5,
	INT_MAX_SIZE
} INT_STREAM;

typedef struct {
	uint32_t checkBit;
	uint32_t handleBit;
	void *data;
	void (*function)(void*);
} INT_HANDLER;

/**
 * Inicjalizacja struktur obsługujących przerwania
 */
void intPrepare();
bool intAddHandler(INT_STREAM stream, INT_HANDLER handler);

#endif
