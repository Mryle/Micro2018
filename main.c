#include "main.h"

void addLedHandlers();

char next_tab[25];
Queue next;
volatile bool clear = false;
volatile bool write = false;


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

	queueInit(&next, next_tab, 25);
	queuePutStr(&next, "Hello:");
	
	write = true;

	LCDconfigure();
	LCDclear();
	// Kontrolna lampka
	ledBlueOn();
	for (;;) {
		if (clear) {
			LCDclear();
			clear = false;
		}
		if (write) {
			char znk = queuePeek(&next);
			queuePop(&next);
			LCDputcharWrap(znk);
			if (queueEmpty(&next)) {
				write = false;
			}
		}
		Delay(36000);
		if (ledBlueGet())
			ledBlueOff();
		else
			ledBlueOn();
	}
}

void keyPressed(uint32_t row, uint32_t col) {
	if (!write) {
		queuePut(&next, 'r');
		queuePut(&next, '0'+row);
		queuePut(&next, 'c');
		queuePut(&next, '0'+col);
		write = true;
	}
	//LCDputcharWrap('0' + row);
	if (ledRedGet()) {
		ledRedOff();
	} else {
		ledRedOn();
	}
}

void keyHolding(uint32_t row, uint32_t col) {
}

