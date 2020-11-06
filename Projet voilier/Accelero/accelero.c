#include "accelero.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_adc.h" 


void confAccelero(void){
	//PC0 -> X et PC1 -> Y a configurer
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
	
	//Configuration de ADC1 et 2, une par axe
	//initaliser les deux structures

	LL_ADC_InitTypeDef * structADC1;
	LL_ADC_InitTypeDef * structADC2;
	
	LL_ADC_Init(ADC1, structADC1);
	LL_ADC_Init(ADC2, structADC2);

	
	
	
}

void getAngleRoulis(void) {
	
}
