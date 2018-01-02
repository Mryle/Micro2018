#include "tim.h"

/*
TIM_TypeDef* timReg[TIM_MAX_SIZE] = {
	TIM2,
	TIM3,
};
*/

void timForceReset(TIM_TypeDef* timer) {
	timer->EGR = TIM_EGR_UG;
}

void timPrepareInternal(TIM_TypeDef* timer, uint32_t psc, uint32_t arr) {
	timer->PSC = psc;
	timer->ARR = arr;
	timForceReset(timer);
}

void timPrepareUp(TIM_TypeDef* timer, uint32_t psc, uint32_t arr) {
	timer->CR1 = 0;
	timPrepareInternal(timer, psc, arr);
}

void timPrepareDown(TIM_TypeDef* timer, uint32_t psc, uint32_t arr) {
	timer->CR1 = TIM_CR1_DIR;
	timPrepareInternal(timer, psc, arr);
}

void timPrepareBoth(TIM_TypeDef* timer, uint32_t psc, uint32_t arr) {
	timer->CR1 = TIM_CR1_CMS_0 | TIM_CR1_CMS_1;
	timPrepareInternal(timer, psc, arr);
}

void timEnable(TIM_TypeDef* timer) {
	timer->CR1 |= TIM_CR1_CEN;
}

void timDisable(TIM_TypeDef* timer) {
	//	timer->CR1 |= TIM_CR1_CEN;
	timer->CR1 = (~TIM_CR1_CEN) & timer->CR1;
}

void timInterruptDefaultEnable(TIM_TypeDef* timer) {
	timer->SR = ~(TIM_SR_UIF | TIM_SR_CC1IF);
	timer->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE;
}
