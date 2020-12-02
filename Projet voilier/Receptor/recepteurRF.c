#include "MyTimer.h"
#include "recepteurRF.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"
#include <math.h>

#define GAUCHE 0
#define DROITE 1


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
	//GPIOB->CRL &= ~(0xF << 28);
	GPIOB->CRL |= (1<< 30);
	
	//Select the active input for TIM4_CCR1: write the CC1S bits to 01 in the TIMx_CCMR1 register (TI1 selected)
	TIM4->CCMR1 &= ~(1<<1);
	TIM4->CCMR1 |= (1<<0);
	
	//Select the active polarity active on rising edge CC1
	//TIM4->CCER |= TIM_CCER_CC1P;
	
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

float getdutyCycle(){
	int pulse, periode; 
	float dutycycle;
	
	periode =  LL_TIM_OC_GetCompareCH1(TIM4);
	pulse =  LL_TIM_OC_GetCompareCH2(TIM4);
	
	dutycycle = ((float) pulse / (float) periode)  ;

	return dutycycle;
	
}

void setRF(){
	float dutycycle = getdutyCycle();
	//7.5% -> valeur neutre et 10% -> max
	float set = (dutycycle - 0.075) / (0.1 - 0.075);
	
	//si set > tourne à gauche
	int sens = (set > 0);
	
	//tolérance de 2.5%
	if (fabs(set) > 0.025) {
		//gauche
		if (sens == GAUCHE) {
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
		} else {
			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2);
		}	
		
		LL_TIM_OC_SetCompareCH2(TIM2, fabs(set));
		
	} else {
		LL_TIM_OC_SetCompareCH2(TIM2, 0);
	}
	
}

