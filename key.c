#include "key.h"
#include "led.h"

#include <delay.h>

#define KEY_GPIO	GPIOC

int keyColPin[] = {0, 1, 2, 3};
int keyRowPin[] = {6, 7, 8, 9};

uint32_t lastRow = 0;

extern void keyPressed(uint32_t row, uint32_t col);

void keyInitInOutConfigure() {
	// Ustawianie wyprowadzeń kolumn
	for(int a = 0; a < 4; a++) {
		GPIOoutConfigure(KEY_GPIO, keyColPin[a], GPIO_OType_PP, GPIO_Low_Speed, GPIO_PuPd_NOPULL);
	}
	// Ustawianie wyprowadzeń wierszy
	for(int a = 0; a < 4; a++) {
		GPIOinConfigure(KEY_GPIO, keyRowPin[a], GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Falling);
	}
}

/**
 * Wyzerowanie znaczników dla przerwań klawiszy
 **/
void keyResetInterrupts() {
	EXTI->PR = EXTI_PR_PR6;
	EXTI->PR = EXTI_PR_PR7;
	EXTI->PR = EXTI_PR_PR8;
	EXTI->PR = EXTI_PR_PR9;
}

/**
 * Rozpoczęcie (z wyzerowaniem) używanego licznika.
 **/
void keyStartTimer() {
}

/*
void keyDisableHandlers() {
	NVIC_DisableIRQ(EXTI9_5_IRQn);
	NVIC_DisableIRQ(TIM2_IRQn);
}
*/

/**
 * Stan niski na liniach kolumn
 **/
void keyColLowState() {
	for(int a = 0; a < 4; a++) {
		KEY_GPIO->BSRRH = 1 << keyColPin[a];
	}
}

/**
 * Stan wysoki na liniach kolumn
 **/
void keyColHighState() {
	for(int a = 0; a < 4; a++) {
		KEY_GPIO->BSRRL = 1 << keyColPin[a];
	}
}


void keyPrepare() {
	timPrepareUp(TIM2, 16000, 100);		//Licznik co 1 ms
	timInterruptDefaultEnable(TIM2);	//Włączenie przerwań licznika
	TIM2->CCR1 = 99;
	// Stan niski na wyprowadzeniach kolumn
	keyColLowState();
	// Ustawianie wyprowadzeń wejścia i wyjścia
	keyInitInOutConfigure();
	// Zresetowanie przerwań
	keyResetInterrupts();
	// Włączenie przerwań
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void keyRowHandler(uint32_t row) {
	lastRow = row;
	NVIC_DisableIRQ(EXTI9_5_IRQn);	// Wyłącz przerwania EXTI dla wierszy
	keyResetInterrupts();		// Wyzeruj znaczniki przerwań dla wierszy
	keyColHighState();			// Wysoki stan na liniach kolumn
	// Wyzerowanie rejestru licznika i uruchomienie go
	timForceReset(TIM2);
	timEnable(TIM2);

}

bool keyScanKeyboard() {
	bool state = false;
	for(int col = 0; col < 4 && !state; col++) {
		KEY_GPIO->BSRRH = 1 << keyColPin[col];	//Ustaw stan niski na odpowiedniej kolumnie
		__NOP();
		__NOP();
		__NOP();
		state = !(KEY_GPIO->IDR & (1 << keyRowPin[lastRow]));
		if (state) {
			keyPressed(lastRow, col);
		}
		KEY_GPIO->BSRRL = 1 << keyColPin[col];	// Ponownie ustawiamy stan wysoki na kolumnie
	}
	return state;
}

// Trwa na tyle długo, że usuwa całość
void keyTimerHandler() {
	timDisable(TIM2);	// Wyłącza licznik
	// Skanuj stan klawiatury
	//keyPressed(lastRow, 0);	//Wywołaj zdarzenie klikniecia przycisku
	
	keyScanKeyboard();
	
	if (true) {		// !keyScanKeyboard()
		keyColLowState();		// Ustawienie niskiego stanu
		keyResetInterrupts();
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}
}
