#include "main.h"

void addLedHandlers();

int main() {
	// Włączenie taktowania.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	__NOP();
	// Przygotowanie poszczególnych modułów.
	intPrepare();
	ledPrepare();
	keyPrepare(TIM_2);

	ledBlueOn();
	for (;;) {
		;
	}
}

