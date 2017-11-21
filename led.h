#ifndef __LED_H
#define __LED_H
#include <gpio.h>
#include <stm32.h>

//GPIO of specific devices

#define LED_RED_GPIO    GPIOA
#define LED_GREEN_GPIO  GPIOA
#define LED_BLUE_GPIO   GPIOB
#define LED_GREEN2_GPIO GPIOA

//PIN/PORT Numbers of devices
#define LED_RED_PIN    6
#define LED_GREEN_PIN  7
#define LED_BLUE_PIN   0
#define LED_GREEN2_PIN 5

#define ledRedOn()		\
				LED_RED_GPIO->BSRRH = 1 << LED_RED_PIN
#define ledRedOff()		\
				LED_RED_GPIO->BSRRL = 1 << LED_RED_PIN
#define ledRedGet()		\
				LED_RED_GPIO->ODR & (1 << LED_RED_PIN)
#define ledGreenOn()	\
				LED_GREEN_GPIO->BSRRH = 1 << LED_GREEN_PIN
#define ledGreenOff()	\
				LED_GREEN_GPIO->BSRRL = 1 << LED_GREEN_PIN
#define ledGreenGet()		\
				LED_GREEN_GPIO->ODR & (1 << LED_GREEN_PIN)
#define ledBlueOn()		\
				LED_BLUE_GPIO->BSRRH = 1 << LED_BLUE_PIN
#define ledBlueOff()	\
				LED_BLUE_GPIO->BSRRL = 1 << LED_BLUE_PIN
#define ledBlueGet()		\
				LED_BLUE_GPIO->ODR & (1 << LED_BLUE_PIN)
#define ledGreen2On()	\
				LED_GREEN2_GPIO->BSRRL = 1 << LED_GREEN2_PIN
#define ledGreen2Off()	\
				LED_GREEN2_GPIO->BSRRH = 1 << LED_GREEN2_PIN
#define ledGreen2Get()		\
				!(LED_GREEN2_GPIO->ODR & (1 << LED_GREEN2_PIN))

void ledPrepare();
#endif
