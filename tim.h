#ifndef __TIM_H
#define __TIM_H
#include <stm32.h>
#include <stdbool.h>
#include "interrupts.h"
/**
 * Wymagane:
 * Włączenie taktowania licznika x:
 * RCC->APB1ENR |= RCC_APB1ENR_TIMxEN;
 */

typedef enum {
	TIM_2	=	0,
	TIM_3	=	1,
	TIM_MAX_SIZE
} TIM_NUM;

/**
 * Inicjalizacja parametrów z zerowaniem licznika
 * psc - prescaler
 * asc - licznik
 * częstotliwość - 16 Mhz = 16*10^6Hz
 */
void timPrepareUp(TIM_NUM num, uint32_t psc, uint32_t arr);
void timPrepareDown(TIM_NUM num, uint32_t psc, uint32_t arr);
void timPrepareBoth(TIM_NUM num, uint32_t psc, uint32_t arr);

/**
 * Włączenie licznika
 */
void timEnable(TIM_NUM num);
void timDisable(TIM_NUM num);
/**
 * Włączenie przerwań
 * nvic = czy włączyć także na poziomie nvic
 */
void timInterruptEnable(TIM_NUM num, bool nvic);
/**
 * Pobranie nazwy przerwania dla licznika
 */
INT_STREAM timInterrupt(TIM_NUM num);

/**
 * Wymuszenie zdarzenia uaktualnienia (żeby np. wyczyścić rejestry)
 */
void timForceReset(TIM_NUM num);

//TODO:
//Zmiana częstotliwości działania licznika (domyślnie jest taktowany z zegarem 16MHz)
#endif
