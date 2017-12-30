#include "led.h"

void ledPrepare() {
	ledRedOff();
	ledGreenOff();
	ledBlueOff();
	ledGreen2Off();
	GPIOoutConfigure(LED_RED_GPIO,
			LED_RED_PIN,
			GPIO_OType_PP,
			GPIO_Low_Speed,
			GPIO_PuPd_NOPULL);
	GPIOoutConfigure(LED_GREEN_GPIO,
			LED_GREEN_PIN,
			GPIO_OType_PP,
			GPIO_Low_Speed,
			GPIO_PuPd_NOPULL);
	GPIOoutConfigure(LED_BLUE_GPIO,
			LED_BLUE_PIN,
			GPIO_OType_PP,
			GPIO_Low_Speed,
			GPIO_PuPd_NOPULL);
	GPIOoutConfigure(LED_GREEN2_GPIO,
			LED_GREEN2_PIN,
			GPIO_OType_PP,
			GPIO_Low_Speed,
			GPIO_PuPd_NOPULL);
}

void ledRedSwitch() {
	if (ledRedGet())
		ledRedOff();
	else
		ledRedOn();
}

void ledGreenSwitch() {
	if (ledGreenGet())
		ledGreenOff();
	else
		ledGreenOn();
}

void ledBlueSwitch() {
	if (ledBlueGet())
		ledBlueOff();
	else
		ledBlueOn();
}
