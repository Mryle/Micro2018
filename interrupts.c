#include "interrupts.h"
#include <irq.h>
#include "led.h"

INT_HANDLER handlers[INT_MAX_SIZE][INT_MAX_HANDLERS];

void intHandleBit(uint32_t handleBit, void *data);
void intHandleNegativeBit(uint32_t handleBit, void *data);

IRQn_Type INT_INTERRUPTS[] = {
	DMA1_Stream6_IRQn,
	DMA1_Stream5_IRQn,
	EXTI0_IRQn,
	EXTI3_IRQn,
	EXTI4_IRQn,
	EXTI9_5_IRQn,
	EXTI15_10_IRQn,
	TIM2_IRQn,
	TIM3_IRQn,
	TIM4_IRQn,
	TIM5_IRQn,
};

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

void intEnable(INT_STREAM stream) {
	NVIC_EnableIRQ(INT_INTERRUPTS[stream]);
}

void intDisable(INT_STREAM stream) {
	NVIC_DisableIRQ(INT_INTERRUPTS[stream]);
}

void intSetPriority(INT_STREAM stream, uint32_t prio, uint32_t subprio) {
	IRQsetPriority(INT_INTERRUPTS[stream], prio, subprio);
}

void intAutoEnable() {
	for(int a = 0; a < INT_MAX_SIZE; a++) {
		for(int b = 0; b < INT_MAX_HANDLERS; b++) {
			handlers[a][b].function = 0;
			if (handlers[a][b].function != 0) {
				NVIC_EnableIRQ(INT_INTERRUPTS[b]);
				break;
			}
		}
	}
}

void intHandle(INT_STREAM stream, volatile uint32_t *check, volatile uint32_t *handle) {
	for(int a = 0; a < INT_MAX_HANDLERS; a++) {
		if (handlers[stream][a].function != 0 && (*check) & handlers[stream][a].checkBit) {
			(*handle) = handlers[stream][a].handleBit;
			handlers[stream][a].function(handlers[stream][a].data);
		}
	}
}

void intTimHandle(INT_STREAM stream, volatile uint32_t check, volatile uint32_t *handle) {
	for(int a = 0; a < INT_MAX_HANDLERS; a++) {
		if (handlers[stream][a].function != 0 && check & handlers[stream][a].checkBit) {
			(*handle) = handlers[stream][a].handleBit;
			handlers[stream][a].function(handlers[stream][a].data);
		}
	}
}

void DMA1_Stream6_IRQHandler() {
	intHandle(INT_DMA1S6, &(DMA1->HISR), &(DMA1->HIFCR));
}

void DMA1_Stream5_IRQHandler() {
	intHandle(INT_DMA1S5, &(DMA1->HISR), &(DMA1->HIFCR));
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


void EXTI15_10_IRQHandler() 
{
	intHandle(INT_EXTI15_10, &(EXTI->PR), &(EXTI->PR));
}

void TIM4_IRQHandler() {
	intTimHandle(INT_TIM4, TIM4->SR & TIM4->DIER, &(TIM4->SR));
}

void TIM5_IRQHandler() {
	intTimHandle(INT_TIM5, TIM5->SR & TIM5->DIER, &(TIM5->SR));
}

