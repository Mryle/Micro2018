#include "key.h"
#include "led.h"

#define KEY_GPIO	GPIOC

int keyColPin[] = {0, 1, 2, 3};
int keyRowPin[] = {6, 7, 8, 9};
TIM_NUM timer;

void keyRowHandler(void *data);

void keyInitInternalInOut() {
	// Ustawianie wyprowadzeń kolumn
	for(int a = 0; a < 4; a++) {
		GPIOoutConfigure(KEY_GPIO, keyColPin[a], GPIO_OType_PP, GPIO_Low_Speed, GPIO_PuPd_NOPULL);
	}
	// Ustawianie wyprowadzeń wierszy
	for(int a = 0; a < 4; a++) {
		GPIOinConfigure(KEY_GPIO, keyRowPin[a], GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Rising_Falling);
	}
}

void keyAddHandler(INT_STREAM stream, int row, uint32_t bit) {
	INT_HANDLER handler;
	handler.checkBit = bit;
	handler.handleBit = bit;
	handler.data = 0;
	handler.function = keyRowHandler;
	intAddHandler(stream, handler);
}

void keyInitInternalHandlers() {
	keyAddHandler(INT_EXTI9_5, 0, EXTI_PR_PR6); 
	keyAddHandler(INT_EXTI9_5, 1, EXTI_PR_PR7); 
	keyAddHandler(INT_EXTI9_5, 2, EXTI_PR_PR8); 
	keyAddHandler(INT_EXTI9_5, 3, EXTI_PR_PR9); 
}

void keyResetInterrupts() {
	// Wyzerowanie przerwań
	EXTI->PR = EXTI_PR_PR6;
	EXTI->PR = EXTI_PR_PR7;
	EXTI->PR = EXTI_PR_PR8;
	EXTI->PR = EXTI_PR_PR9;
}

void keyStartTimer() {
	timForceReset(timer);
	timEnable(timer);
}

void keyEnableHandlers() {
	intEnable(INT_EXTI9_5);
}

void keyDisableHandlers() {
	intDisable(INT_EXTI9_5);
}

void keyPrepare(TIM_NUM tim) {
	// Ustawienie i konfiguracja licznika.
	// timer = tim;
	// timPrepareUp(tim, 16000, 10); //Licznik co 10 ms
	// Stan niski na wyprowadzeniach kolumn
	for(int a = 0; a < 4; a++) {
		KEY_GPIO->BSRRH = 1 << keyColPin[a];
	}
	// Ustawianie wyprowadzeń wejścia i wyjścia
	keyInitInternalInOut();
	// Dodanie obsługi przerwań
	keyInitInternalHandlers();
	// Zresetowanie przerwań
	keyResetInterrupts();
	// Włączenie przerwań
	keyEnableHandlers();
}

void keyRowHandler(void *data) {
	if (ledRedGet()) {
		ledRedOff();
	} else {
		ledRedOn();
	}
}
