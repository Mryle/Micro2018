#include <irq.h>
#include "buttons.h"
#include "rwr.h"

//GPIO of specific devices
#define DISC_KEY_GPIO		GPIOC
#define JOY_KEY_GPIO		GPIOB
#define EXP_KEY_GPIO		GPIOA

//PIN/PORT Numbers of devices
#define USER_KEY_PIN	13
#define LEFT_KEY_PIN	3
#define RIGHT_KEY_PIN	4
#define UP_KEY_PIN		5
#define DOWN_KEY_PIN	6
#define	ACC_KEY_PIN		10
#define ATM_KEY_PIN		0

ButState buttons[BUT_MAX_SIZE] =
{
		{false,	USER_KEY_PIN,	DISC_KEY_GPIO,	"user"},
		{false,	LEFT_KEY_PIN,	JOY_KEY_GPIO,		"left"},
		{false,	RIGHT_KEY_PIN,JOY_KEY_GPIO,		"right"},
		{false,	UP_KEY_PIN,		JOY_KEY_GPIO,		"up"},
		{false,	DOWN_KEY_PIN,	JOY_KEY_GPIO,		"down"},
		{false,	ACC_KEY_PIN,	JOY_KEY_GPIO,		"acc"},
		{true,	ATM_KEY_PIN,	EXP_KEY_GPIO,		"atm"},
};

ButState* butGetInfo(BUT_TYPE type) {
	return &buttons[type];
}

void butClearButtons() {
	/*
	buttons[BUT_USER].state	= !ReadKeyState(DISC_KEY_GPIO, USER_KEY_PIN);
	buttons[BUT_LEFT].state	= !ReadKeyState(JOY_KEY_GPIO, LEFT_KEY_PIN);
	buttons[BUT_RIGHT].state= !ReadKeyState(JOY_KEY_GPIO, RIGHT_KEY_PIN);
	buttons[BUT_UP].state		= !ReadKeyState(JOY_KEY_GPIO, UP_KEY_PIN);
	buttons[BUT_DOWN].state	= !ReadKeyState(JOY_KEY_GPIO, DOWN_KEY_PIN);
	buttons[BUT_ACC].state	= !ReadKeyState(JOY_KEY_GPIO, ACC_KEY_PIN);
	buttons[BUT_ATM].state	=  ReadKeyState(EXP_KEY_GPIO, ATM_KEY_PIN);
	*/
}

void butConfigureInterrupts() {
GPIOinConfigure(DISC_KEY_GPIO, USER_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling);
GPIOinConfigure(JOY_KEY_GPIO, LEFT_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling);
GPIOinConfigure(JOY_KEY_GPIO, RIGHT_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling);
GPIOinConfigure(JOY_KEY_GPIO, UP_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling);
GPIOinConfigure(JOY_KEY_GPIO, DOWN_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
	 	EXTI_Trigger_Rising_Falling);
GPIOinConfigure(JOY_KEY_GPIO, ACC_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
	 	EXTI_Trigger_Rising_Falling);
GPIOinConfigure(EXP_KEY_GPIO, ATM_KEY_PIN, GPIO_PuPd_UP, EXTI_Mode_Interrupt,
	 	EXTI_Trigger_Rising_Falling);
}

