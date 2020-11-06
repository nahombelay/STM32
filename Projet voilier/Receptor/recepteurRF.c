#include "MyTimer.h"
#include "recepteurRF.h"


void recepteurRFConf() {
	//deux registres pour pwm télécommande car on s'interesse au ratio ccr2/ccr1 -> % entre 5 et 10
	// PB6 ccr1 -> véritable période et PB7 ccr2 -> alpha
	// arr légèrement > 20 ms, vers 21
	// 7,45 - 7,55 -> valeur neutre
	
	int Arr = 16599;
	int Psc = 98;
	MyTimer_Conf(TIM4, Arr, Psc);
	
	//Configuration des broches pb6 et pb7
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	//PB6: en floating input (à vérifier)
	GPIOB->CRL &= ~(0xF << 24);
	GPIOB->CRL |= (1<< 26);
	
	//PB7: en floating input (à vérifier)
	GPIOB->CRL &= ~(0xF << 28);
	GPIOB->CRL |= (1<< 30);
	
	//Select the active input for TIM4_CCR1: write the CC1S bits to 01 in the TIMx_CCMR1 register (TI1 selected)
	TIM4->CCMR1 &= ~(1<<1);
	TIM4->CCMR1 |= (1<<0);
	
	//Select the active polarity active on rising edge CC1
	TIM4->CCER |= TIM_CCER_CC1P;
	
	//Select the active input for TIM4_CCR2: write the CC2S bits to 10 in the TIMx_CCMR1 register (TI1 selected)
	TIM4->CCMR1 &= ~(1<<8);
	TIM4->CCMR1 |= (1<<9);
	
	//Select the active polarity active on rising edge CC2
	TIM4->CCER |= TIM_CCER_CC2P;
	
	//Set ts bits to 101 -> Filtered timer input
	TIM4->SMCR &= ~(0x7<<4); 
	TIM4->SMCR |= (5<<4); 
	
	//Set SMS bits to 100 -> Reset mode
	TIM4->SMCR &= ~(0x7); 
	TIM4->SMCR |= (1<<2);

	//Capture enable
	TIM4->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
	

	//CC1S -> 01 input IC1 mapped in TI1
	// 			si 10 mapped sur TI2	
	//configuration de TIM4 sur PB6 et PB7 pour le récépteur RF	
}

float getAlpha(){
	float periode, alpha; 
	float ratio;
	
	periode = (float) TIM4->CCR1;
	alpha = (float) TIM4->CCR2;
	
	ratio = (alpha / periode) * 100 ;

	return ratio;
	
}

int getSens(void){
	float alpha = getAlpha();
	int sens;
	if ((5.0 <= alpha) && (alpha <= 7.45)){
		 sens = 0;
	} else if ((7.55 <= alpha) && (alpha <= 10.0)) {
		sens = 1;
	}
	return sens;
}

int getPWMmoteur(){
    return (TIM4->ARR)*getAlpha()/100;
}