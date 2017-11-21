#include "tim.h"

TIM_TypeDef* timReg[TIM_MAX_SIZE] = {
	TIM2,
	TIM3,
};

INT_STREAM timInt[TIM_MAX_SIZE] = {
	INT_TIM2,
	INT_TIM3,
};

void timPrepareInternal(TIM_NUM num, uint32_t psc, uint32_t arr) {
	timReg[num]->PSC = psc;
	timReg[num]->ARR = arr;
	timReg[num]->EGR = TIM_EGR_UG; // Wymuszenie zdarzenia uaktualnienia
}

void timPrepareUp(TIM_NUM num, uint32_t psc, uint32_t arr) {
	timReg[num]->CR1 = 0;
	timPrepareInternal(num, psc, arr);
}

void timPrepareDown(TIM_NUM num, uint32_t psc, uint32_t arr) {
	timReg[num]->CR1 = TIM_CR1_DIR;
	timPrepareInternal(num, psc, arr);
}

void timPrepareBoth(TIM_NUM num, uint32_t psc, uint32_t arr) {
	timReg[num]->CR1 = TIM_CR1_CMS_0 | TIM_CR1_CMS_1;
	timPrepareInternal(num, psc, arr);
}

void timEnable(TIM_NUM num) {
	timReg[num]->CR1 |= TIM_CR1_CEN;
}

void timInterruptEnable(TIM_NUM num, bool nvic) {
	timReg[num]->SR = ~(TIM_SR_UIF | TIM_SR_CC1IF);
	timReg[num]->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE;
	if (nvic) {
		intEnable(timInterrupt(num));
	}
}

INT_STREAM timInterrupt(TIM_NUM num) {
	return timInt[num];
}