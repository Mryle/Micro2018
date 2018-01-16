#include "main.h"

char tabmask[4][4][4] = {
{	{'1',	'A',	'D',	 8 },
	{'G',	'J',	'M',	 1 },
	{'P',	'T',	'W',	 2 },
	{'*',	'0',	'#',	 0 }},

{	{'1',	'B',	'E',	 8 },
	{'H',	'K',	'N',	 1 },
	{'R',	'U',	'X',	 2 },
	{'*',	' ',	'.',	 0 }},

{	{'1',	'C',	'F',	 8 },
	{'I',	'L',	'O',	 1 },
	{'S',	'V',	'Y',	 2 },
	{'*',	'0',	'?',	 0 }},

{	{'1',	'2',	'3',	 8 },
	{'4',	'5',	'6',	 1 },
	{'7',	'8',	'9',	 2 },
	{'*',	' ',	'!',	 0 }}
};

volatile bool write = false;
volatile bool preview = false;
volatile char previewChar = 0;
char acComm = 0;
char prev = 0;

/**
 * Obsługa wypisywania klawiszy
 **/

#define textSize 128
char text[textSize];
int16_t textLength = 0;
int16_t line = 0;	// Aktualne przesunięcie ekrnau w liniach
int16_t pos = 0;

void MoveToPosition() {
	int linemov = pos / LCDgetmaxcol() - line;
	LCDgoto(linemov, pos % LCDgetmaxcol());
}

void WritePreview() {
	MoveToPosition();
	if (preview) {
		LCDputcharspecwrap(previewChar);
		preview = false;
	} else {
		LCDputcharspecwrap(' ');
	}
}

void WriteChar(char c) {
	MoveToPosition();
	LCDputcharWrap(c);
}

void WriteRedrawScreen() {
	LCDclear();
	int16_t _pos = line * LCDgetmaxcol();
	for(; _pos < textSize; _pos++) {
			LCDputcharWrap(text[_pos]);
	}
}

/**
 * Sprawdza czy trzeba przerysować ekran i jak tak to przerysowuje
 **/
bool CheckRedrawLineCorrect() {
	int linemov = pos/LCDgetmaxcol() - line;
	if (linemov < 0) {
		line += 3;
	} else if (linemov > LCDgetmaxline()) {
		line -= 3;
	} else {
		return true;
	}
	WriteRedrawScreen();
	return false;
}


void WriteNewChar(char t) {
	if (pos < textSize - 1) {
		text[pos] = t;
		if (CheckRedrawLineCorrect()) {
			WriteChar(text[pos]);
		}
		pos++;
		if (textLength < pos) {
			textLength++;
		}
	}
}

void WriteDeleteChar() {
	if (pos > 0) {
		for(int a = pos; a + 1 < textSize; a++) {
			text[a] = text[a+1];
		}
		text[textSize - 1] = ' ';
		textLength--;
		pos--;
		if (CheckRedrawLineCorrect()) {
			if (pos < textLength) {
				WriteRedrawScreen();
			} else {
				pos++;
				WriteChar(' '); //Usunięcie poprzedniego priview
				pos--;
			}
		}
	}
}

void WriteClear() {
	for(int a = 0; a < textSize; a++) {
		text[a] = ' ';
	}
	textLength = 0;
	pos = 0;
	LCDclear();
}

/**
 * Obsługa przerwań z klawiatury
 **/

volatile uint32_t row = 5, col = 5, click = 0;

static void _resetKey() {
	row = 5;
	col = 5;
	click = -1;
}

static void acceptKey() {
	click = click % 4;
	if (row != 5) {
		char znk = tabmask[click][row][col];
		if (!write) {
			acComm = znk;
			write = true;
			previewChar = ' ';
			preview = true;
		}
	}
	_resetKey();
}

void keyPressed(uint32_t _row, uint32_t _col) {
	timDisable(TIM3);
	// Sprawdzanie funkcji
	if (row == _row && col == _col) {
		click++;
		click %= 4;
	} else {
		if (click != -1) {
			acceptKey();
		}
		row = _row;
		col = _col;
		click = 0;
	}
	char znk = tabmask[click][row][col];
	if (znk <= 8) {
		if (!write) {
			acComm = znk;
			write = true;
		} else { // Aktualnie już edytujemy jakiś znak
			if (znk == 0) { // Chcemy wyczyścić ekran, zróbmy to
				acComm = 0;
			} else if (znk == 8) { //Chcemy usunąć znak, więc nie wypisujmy nastepnego
				write = false;
			}
		}
		_resetKey();
	} else {
		// Preview
		previewChar = znk;
		preview = true;

		timForceReset(TIM3);
		timEnable(TIM3);
	}
}

void keyLongTimer() {
	timDisable(TIM3);	// Wyłączamy ten licznik
	acceptKey();
}

void prepareSecondTim() {
	timPrepareUp(TIM3, 16000, 500);		//Licznik co 1 ms
	timInterruptDefaultEnable(TIM3);	//Włączenie przerwań licznika
	TIM3->CCR1 = 499;
	NVIC_EnableIRQ(TIM3_IRQn);
}

void drawLoop() {
	if (write) {
		char znk = acComm;
		if (znk == 0) {
			WriteClear();
		} else if (znk == 1) {
			WriteChar(text[pos]);	// Clearing Preview
			if (pos > 0) {
				pos--;
			}
		} else if (znk == 2) {
			WriteChar(text[pos]); // Clearing Preview
			if (pos < textLength) pos++;
		} else if (znk == 8) {
			WriteDeleteChar();
		} else if (znk > 8) {
			WriteNewChar(znk);
		}
		WritePreview();
		write = false;
	}
	if (preview) {
		WritePreview();
	}
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
	
	prepareSecondTim();

	IRQsetPriority(TIM2_IRQn, 		HIGH_IRQ_PRIO,	HIGH_IRQ_SUBPRIO);
	IRQsetPriority(TIM3_IRQn, 		HIGH_IRQ_PRIO,	MIDDLE_IRQ_SUBPRIO);
	IRQsetPriority(EXTI9_5_IRQn,	HIGH_IRQ_PRIO,	VERY_HIGH_IRQ_SUBPRIO);
	
	write = true;

	LCDconfigure();
	WriteClear();

	// Kontrolna lampka
	ledBlueOn();
	for (;;) {
		drawLoop();
	}
}

void EXTI9_5_IRQHandler() // Handler
{
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
		keyLongTimer();
	}
	if (val & TIM_SR_UIF) {
		TIM3->SR = ~TIM_SR_UIF;
	}
}
