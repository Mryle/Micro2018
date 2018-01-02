#ifndef __TIM_H
#define __TIM_H
#include <stm32.h>
#include <stdbool.h>
/**
 * Wymagane:
 * Włączenie taktowania licznika x:
 * RCC->APB1ENR |= RCC_APB1ENR_TIMxEN;
 */

/**
 * Inicjalizacja parametrów z zerowaniem licznika
 * psc - prescaler
 * asc - licznik
 * częstotliwość - 16 Mhz = 16*10^6Hz
 */
void timPrepareUp(TIM_TypeDef* timer, uint32_t psc, uint32_t arr);
void timPrepareDown(TIM_TypeDef* timer, uint32_t psc, uint32_t arr);
void timPrepareBoth(TIM_TypeDef* timer, uint32_t psc, uint32_t arr);

/**
 * Włączenie licznika
 */
void timEnable(TIM_TypeDef* timer);
void timDisable(TIM_TypeDef* timer);
/**
 * Włączenie przerwań UIF i CC1IF na poziomie licznika
 */
void timInterruptDefaultEnable(TIM_TypeDef* timer);
/**
 * Ustawia flagę wymuszającą zdarzenie uaktualnienia (żeby np. wyczyścić rejestry)
 *
 */
void timForceReset(TIM_TypeDef* timer);

//TODO:
//Zmiana częstotliwości działania licznika (domyślnie jest taktowany z zegarem 16MHz)
#endif
