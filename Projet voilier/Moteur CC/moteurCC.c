#include "MyTimer.h"
#include "moteurCC.h"
#include "recepteurRF.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_tim.h" 



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
	
	GPIOA->CRL &= ~(0xF << 8); // PA.2 output push-pull
	GPIOA->CRL |= (0x2 << 8); // PA.2 gp push pull
	
	//Configuration de CH2 en mode PWM cf doc page 348
	TIM2->CCMR1 &= ~(0x7<<12);
	TIM2->CCMR1 |= (0x6<<12);
	
	//CCER 
	TIM2->CCER &= ~TIM_CCER_CC1P;
	TIM2->CCER |= TIM_CCER_CC2P;

	
	//SMCR TS à mettre à 101
	TIM2->SMCR |= TIM_SMCR_TS_0 | TIM_SMCR_TS_2;
	
	//SMCR SMS à 100
	TIM2->SMCR |= TIM_SMCR_SMS_2; //100 
	
	TIM2->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
}

void tournerTable() {
	setRF();
		
}
