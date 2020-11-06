#include "MyTimer.h"
#include "moteurCC.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_tim.h" 

#define GAUCHE 0
#define DROITE 1

//May need to review the values of GAUCHE and DROITE

void moteurccConf() {
	
	//Broches: PA1 -> PWM sur timer 2 CH2 et PA2 pour le sens
	int Arr = 0xE0F;
	int Psc = 0;
	MyTimer_Conf(TIM2, Arr, Psc);
	//TIM2 configuré avec frequence de 20kHz
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // validation de l'horloge de PA
	
	GPIOA->CRL &= ~(0xF << 4); // PA.1 alternate push-pull
	GPIOA->CRL |= (0xA << 4); // PA.1 alternate push-pull
	
	GPIOA->CRL &= ~(0xF << 8); // PA.2 alternate push-pull
	GPIOA->CRL |= (0xA << 8); // PA.2 alternate push-pull
	
	//Configuration de CH2 en mode PWM cf doc page 348
	TIM2->CCMR1 &= ~(0x7<<12);
	TIM2->CCMR1 |= (0x6<<12);
	
	TIM2->CCER |= TIM_CCER_CC2E;
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
}

void tournerTable(int pwmInput, int sens) {
	//PA1->pwm and PA2->Sens
	
	if (sens == GAUCHE) {
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	} else {
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
	}	
	LL_TIM_OC_SetCompareCH2(TIM2, pwmInput);
		
}
