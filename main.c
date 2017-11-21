#include "main.h"

void addLedHandlers();

bool status[2];

int main() {
	// Inicjalizacja
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	__NOP();
	// Turn In RCC & Cleaning
	ledPrepare();
	intPrepare();
	timPrepareUp(TIM_3, 16000, 1000);
	timPrepareUp(TIM_2, 16000,  500);
	//TIM3->CCR1 = 249;
	addLedHandlers();
	timInterruptEnable(TIM_2, true);
	timInterruptEnable(TIM_3, true);
	timEnable(TIM_3);
	timEnable(TIM_2);
	//ledBlueOn();
	for (;;) {
		;
	}
}

void switchRedLed(void *data) {
	if (ledRedGet()) {
		ledRedOff();
	} else {
		ledRedOn();
	}
}

void switchGreenLed(void *data) {
	if (ledGreenGet()) {
		ledGreenOff();
	} else {
		ledGreenOn();
	}
}

void addLedHandlers() {
	INT_HANDLER handler;
	handler.checkBit = TIM_SR_UIF;
	handler.handleBit = ~TIM_SR_UIF;
	handler.data = &status[0];
	handler.function = switchRedLed;
	intAddHandler(INT_TIM2, handler);
	handler.checkBit = TIM_SR_UIF;
	handler.handleBit = ~TIM_SR_UIF;
	handler.data = &status[1];
	handler.function = switchGreenLed;
	intAddHandler(INT_TIM3, handler);
}

