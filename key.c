#include "key.h"
#include "led.h"

#define KEY_GPIO	GPIOC

int keyColPin[] = {0, 1, 2, 3};
int keyRowPin[] = {6, 7, 8, 9};

uint32_t lastRow = 0;
uint32_t holding = 0;

extern void keyPressed(uint32_t row, uint32_t col);
extern void keyHolding(uint32_t row, uint32_t col, uint32_t holding);

void keyRowHandler(void *data);
void keyTimerHandler(void *data);

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

void keyAddHandler(INT_STREAM stream, uint32_t row, uint32_t bit) {
	INT_HANDLER handler;
	handler.checkBit = bit;
	handler.handleBit = bit;
	handler.data = (void*) row;
	handler.function = keyRowHandler;
	intAddHandler(stream, handler);
}

void keyInitInternalHandlers() {
	keyAddHandler(INT_EXTI9_5, 0, EXTI_PR_PR6); 
	keyAddHandler(INT_EXTI9_5, 1, EXTI_PR_PR7); 
	keyAddHandler(INT_EXTI9_5, 2, EXTI_PR_PR8); 
	keyAddHandler(INT_EXTI9_5, 3, EXTI_PR_PR9); 
	INT_HANDLER handler;
	handler.checkBit = TIM_SR_CC1IF;
	handler.handleBit = ~TIM_SR_CC1IF;
	handler.data = 0;
	handler.function = keyTimerHandler;
	intAddHandler(timInterrupt(TIM_2), handler);
}

void keyResetInterrupts() {
	// Wyzerowanie przerwań
	EXTI->PR = EXTI_PR_PR6;
	EXTI->PR = EXTI_PR_PR7;
	EXTI->PR = EXTI_PR_PR8;
	EXTI->PR = EXTI_PR_PR9;
}

void keyStartTimer() {
	timForceReset(TIM_2);
	timEnable(TIM_2);
}

void keyEnableHandlers() {
	intEnable(INT_EXTI9_5);
}

void keyDisableHandlers() {
	intDisable(INT_EXTI9_5);
}

void keyColLowState() {
	for(int a = 0; a < 4; a++) {	//Stan niski na liniach kolumn
		KEY_GPIO->BSRRH = 1 << keyColPin[a];
	}
}

void keyColHighState() {
	for(int a = 0; a < 4; a++) {	//Stan wysoki na liniach kolumn
		KEY_GPIO->BSRRL = 1 << keyColPin[a];
	}
}

void keyPrepare() {
	// Ustawienie i konfiguracja licznika.
	timPrepareUp(TIM_2, 16000, 500);		//Licznik co 10 ms
	timInterruptEnable(TIM_2, true);	//Włączenie przerwań licznika
	TIM2->SR = ~(TIM_SR_UIF | TIM_SR_CC1IF);
	TIM2->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE;
	TIM2->CCR1 = 499;
	// Stan niski na wyprowadzeniach kolumn
	keyColLowState();
	// Ustawianie wyprowadzeń wejścia i wyjścia
	keyInitInOutConfigure();
	// Dodanie obsługi przerwań
	keyInitInternalHandlers();
	// Zresetowanie przerwań
	keyResetInterrupts();
	// Włączenie przerwań
	keyEnableHandlers();
}

void keyRowHandler(void *data) {
	lastRow = (uint32_t) data;
	keyDisableHandlers();		// Wyłącz przerwania EXTI dla wierszy
	keyResetInterrupts();		// Wyzeruj znaczniki przerwań dla wierszy
	keyColHighState();
	keyStartTimer();	// Wyzerowanie rejestru licznika i uruchomienie go
}

bool keyScanKeyboard() {
	bool state = false;
	for(int col = 0; col < 4; col++) {
		KEY_GPIO->BSRRH = 1 << keyColPin[col]; // Ustaw stan niski na wyprowadzeniach kolumn
		state = (KEY_GPIO->ODR & (1 << keyColPin[col]));
		KEY_GPIO->BSRRH = 1 << keyColPin[col]; // Ustaw stan wysoki na wyprowadzeniach kolumn
		if (state) {
			if (holding > 0) {
				keyHolding(lastRow, col, holding);
				holding ++;
			} else {
				keyPressed(lastRow, col);
				holding = 1;
			}
			return true;
		}
	}
	return false;
}

void keyTimerHandler(void *data) {
	// Skanuj stan klawiatury
	if (!keyScanKeyboard()) {
		holding = 0;
		timDisable(TIM_2);	// Wyłącza licznik
		keyColLowState();		// Ustawienie niskiego stanu
		keyResetInterrupts();				
		keyEnableHandlers();	// Ponownie włącz zdarzenia w układzie EXTI
	}

}
