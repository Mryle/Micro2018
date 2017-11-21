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
	INT_DMA1S5		=	1,
	INT_EXTI0		=	2,
	INT_EXTI3		=	3,
	INT_EXTI4		=	4,
	INT_EXTI9_5		=	5,
	INT_EXTI15_10	=	6,
	INT_TIM2		=	7,
	INT_TIM3		=	8,
	INT_TIM4		=	8,
	INT_TIM5		=	8,
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

void intEnable(INT_STREAM stream);

void intDisable(INT_STREAM stream);

void intSetPriority(INT_STREAM stream, uint32_t prio, uint32_t subprio);

/*
 * Włącza wszystkie przerwania które mają włączony przynajmniej jeden handler
 */
void intAutoEnable();



#endif
