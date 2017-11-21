#include "main.h"

void handleButton(void* data);
void addHandlerButton(BUT_TYPE type, INT_STREAM stream, uint32_t checkBit, uint32_t handleBit);

int main() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	__NOP();
	// Turn In RCC & Cleaning
	ledPrepare();
	intPrepare();
	wrPrepare();
	butClearButtons();
	butConfigureInterrupts();
	//Włączenie przerwań
	addHandlerButton(BUT_USER,	INT_EXTI15_10, EXTI_PR_PR13, EXTI_PR_PR13);
	addHandlerButton(BUT_LEFT,	INT_EXTI3, EXTI_PR_PR3, EXTI_PR_PR3);
	addHandlerButton(BUT_RIGHT, INT_EXTI4, EXTI_PR_PR4, EXTI_PR_PR4);
	addHandlerButton(BUT_UP, 		INT_EXTI9_5, EXTI_PR_PR5, EXTI_PR_PR5);
	addHandlerButton(BUT_DOWN, 	INT_EXTI9_5, EXTI_PR_PR6, EXTI_PR_PR6);
	addHandlerButton(BUT_ACC, 	INT_EXTI15_10, EXTI_PR_PR10, EXTI_PR_PR10);
	addHandlerButton(BUT_ATM, 	INT_EXTI0, EXTI_PR_PR0, EXTI_PR_PR0);
	IRQsetPriority(EXTI0_IRQn, LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	IRQsetPriority(EXTI3_IRQn, LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	IRQsetPriority(EXTI4_IRQn ,LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	IRQsetPriority(EXTI9_5_IRQn ,LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	IRQsetPriority(EXTI15_10_IRQn ,LOW_IRQ_PRIO, VERY_HIGH_IRQ_SUBPRIO);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	wrSend("Working\r\n");
	for (;;) {
		;
	}
}

void addHandlerButton(BUT_TYPE type, INT_STREAM stream, uint32_t checkBit, uint32_t handleBit) {
	INT_HANDLER data;
	data.checkBit = checkBit;
	data.handleBit = handleBit;
	data.data = butGetInfo(type);
	data.function = &handleButton;
	intAddHandler(stream, data);
}

void handleButton(void* data) {
	ButState *ptr = (ButState*)data;
	bool press = false;
	if (ptr->actOnHigh) {
		press = ReadKeyState(ptr->gpio, ptr->pin);
	} else {
		press = !ReadKeyState(ptr->gpio, ptr->pin);
	}
		if (ptr->name[0] == 'd' && ptr->name[1] == 'o') {
			ledRedOn();
		} else if (ptr->name[0] == 'u' && ptr->name[1] == 'p') {
			ledRedOff();
		}
		wrSend(ptr->name);
		if (press) {
			wrSend("_Pressed\r\n");
		} else {
			wrSend("_Released\r\n");
		}
}

