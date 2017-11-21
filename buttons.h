#ifndef __BUT_H
#define __BUT_H
#include <gpio.h>
#include <stm32.h>
#include <stdbool.h>

typedef enum {
	BUT_USER		= 0,
	BUT_LEFT		= 1,
	BUT_RIGHT	= 2,
	BUT_UP			=	3,
	BUT_DOWN		=	4,
	BUT_ACC		=	5,
	BUT_ATM		=	6,
	BUT_MAX_SIZE
} BUT_TYPE;

typedef struct {
	bool actOnHigh;
	uint32_t pin;
	GPIO_TypeDef * gpio;
	char *name;
} ButState;

ButState* butGetInfo(BUT_TYPE type);
void butClearButtons();
void butConfigureInterrupts();

#define ReadKeyState(x, y) \
	((1 << y) & (x->IDR))

#endif
