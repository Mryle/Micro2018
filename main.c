#include "main.h"

char tabmask[4][4][4] = {
{	{'1',	'A',	'D',	'#'},
	{'G',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'}},
{	{'1',	'B',	'E',	'#'},
	{'H',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'}},
{	{'1',	'C',	'F',	'#'},
	{'I',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'},
	{'#',	'#',	'#',	'#'}},
{	{'1',	'2',	'3',	'A'},
	{'4',	'5',	'6',	'B'},
	{'7',	'8',	'9',	'C'},
	{'*',	'\0',	'#',	'D'}}
};


char next_tab[25];
Queue next;
volatile bool clear = false;
volatile bool write = false;
volatile uint32_t row = 5, col = 5, click = 0;

void keyPressed(uint32_t _row, uint32_t _col) {
	if (row == _row && col == _col) {
		click++;
	} else {
		row = _row;
		col = _col;
		click = 1;
	}
	if (!write) {
		queuePut(&next, '.');
		queuePut(&next, tabmask[col][row][0]);
		write = true;
	} else {
		ledRedSwitch();
	}
	//timDisable(TIM_3);
	//timForceReset(TIM_3);
	//timEnable(TIM_3);
}

void onSecondTim() {
	/*timDisable(TIM3);	// Wyłączamy ten licznik
	click = click % 4;
	if (row != 5) {
		char znk = 'A';//tabmask[click][row][col];
		if (znk == '\0') {
			clear = true;
		} else if (!write) {
			queuePut(&next, znk);
			write = true;
		}
	} else if (!write) {
		queuePut(&next, '!');
		write = true;
	}
	row = 5;
	col = 5;
	//LCDputcharWrap('0' + row);
	*/
}

void prepareSecondTim() {
	timPrepareUp(TIM3, 16000, 1000);		//Licznik co 1 ms
	timInterruptDefaultEnable(TIM3);	//Włączenie przerwań licznika
	TIM3->CCR1 = 999;
	NVIC_EnableIRQ(TIM3_IRQn);
}

int main() {
	// Włączenie taktowania.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	__NOP();
	// Przygotowanie poszczególnych modułów.
	ledPrepare();	//	Konfiguracja Out dla ledów i wyłączenie ich
	keyPrepare(); //Just Testing led interrupts
	
	//prepareSecondTim();

	IRQsetPriority(TIM2_IRQn, 		HIGH_IRQ_PRIO,	HIGH_IRQ_SUBPRIO);
	//IRQsetPriority(TIM3_IRQn, 		HIGH_IRQ_PRIO,	MIDDLE_IRQ_SUBPRIO);
	IRQsetPriority(EXTI9_5_IRQn,	HIGH_IRQ_PRIO,	VERY_HIGH_IRQ_SUBPRIO);
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
		Delay(360000);
		if (ledBlueGet())
			ledBlueOff();
		else
			ledBlueOn();
		;
	}
}

void EXTI9_5_IRQHandler() // Handler
{
	//intHandle(INT_EXTI9_5, &(EXTI->PR), &(EXTI->PR));
	uint32_t val = EXTI->PR;
	if (val & EXTI_PR_PR6) {
		EXTI->PR = EXTI_PR_PR6;
		keyRowHandler(0);
	}
	if (val & EXTI_PR_PR7) {
		EXTI->PR = EXTI_PR_PR7;
		keyRowHandler(1);
	}
	if (val & EXTI_PR_PR8) {
		EXTI->PR = EXTI_PR_PR8;
		keyRowHandler(2);
	}
	if (val & EXTI_PR_PR9) {
		EXTI->PR = EXTI_PR_PR9;
		keyRowHandler(3);
	}
}

// Correct Timer handlers
void TIM2_IRQHandler() {	// Handler
	uint32_t val = TIM2->SR & TIM2->DIER;
	if (val & TIM_SR_CC1IF) {
		TIM2->SR = ~TIM_SR_CC1IF;
		keyTimerHandler();
	}
	if (val & TIM_SR_UIF) {
		TIM2->SR = ~TIM_SR_UIF;
	}
}

void TIM3_IRQHandler() {
	uint32_t val = TIM3->SR & TIM3->DIER;
	if (val & TIM_SR_CC1IF) {
		TIM3->SR = ~TIM_SR_CC1IF;
		ledGreenSwitch();
	}
	if (val & TIM_SR_UIF) {
		TIM3->SR = ~TIM_SR_UIF;
	}
}
