#include "interrupts.h"

INT_HANDLER handlers[INT_MAX_SIZE][INT_MAX_HANDLERS];

void intPrepare() {
	for(int a = 0; a < INT_MAX_SIZE; a++) {
		for(int b = 0; b < INT_MAX_HANDLERS; b++) {
			handlers[a][b].function = 0;
		}
	}
}

bool intAddHandler(INT_STREAM stream, INT_HANDLER handler) {
	for(int a = 0; a < INT_MAX_HANDLERS; a++) {
		if (handlers[stream][a].function == 0) {
			handlers[stream][a] = handler;
			return true;
		}
	}
	return false;
}

void intHandle(INT_STREAM stream, volatile uint32_t *check, volatile uint32_t *handle) {
	for(int a = 0; a < INT_MAX_HANDLERS; a++) {
		if (handlers[stream][a].function != 0 &&
				(*check) & handlers[stream][a].checkBit) {
			(*handle) = handlers[stream][a].handleBit;
			handlers[stream][a].function(handlers[stream][a].data);
		}
	}
}

void DMA1_Stream6_IRQHandler() {
	intHandle(INT_DMA1S6, &(DMA1->HISR), &(DMA1->HIFCR));
}

void EXTI0_IRQHandler() 
{
	intHandle(INT_EXTI0, &(EXTI->PR), &(EXTI->PR));
}

void EXTI3_IRQHandler() 
{
	intHandle(INT_EXTI3, &(EXTI->PR), &(EXTI->PR));
}

void EXTI4_IRQHandler() 
{
	intHandle(INT_EXTI4, &(EXTI->PR), &(EXTI->PR));
}

void EXTI9_5_IRQHandler() 
{
	intHandle(INT_EXTI9_5, &(EXTI->PR), &(EXTI->PR));
}

void EXTI15_10_IRQHandler() 
{
	intHandle(INT_EXTI15_10, &(EXTI->PR), &(EXTI->PR));
}

