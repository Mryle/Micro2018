#include "main.h"

void addLedHandlers();

int main() {
	// Włączenie taktowania.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	__NOP();
	// Przygotowanie poszczególnych modułów.
	intPrepare();
	ledPrepare();
	keyPrepare();

	LCDconfigure();
	LCDclear();

	// Kontrolna lampka
	ledBlueOn();
	for (;;) {
		;
	}
}

void keyPressed(uint32_t row, uint32_t col) {
	/*
	if (ledRedGet()) {
		ledRedOff();
	} else {
		ledRedOn();
	}
	*/
}

void keyHolding(uint32_t row, uint32_t col) {
}

