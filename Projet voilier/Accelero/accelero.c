#include "accelero.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_adc.h" 

#define X LL_ADC_CHANNEL_10
#define Y LL_ADC_CHANNEL_11


void confAccelero(void){
	//PC0 -> X et PC1 -> Y a configurer
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; // passage de l'horloge ADC1 à 12MHz
	
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
	
	//Configuration de ADC1 et 2, une par axe
	//initaliser les deux structures

	LL_ADC_InitTypeDef structADC;
	
	//single conversion mode
	structADC.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
	
	//data alignment
	structADC.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT ;
	
	LL_ADC_Init(ADC1, &structADC);
	
	//enable the  ADC
	LL_ADC_Enable(ADC1);
	
	//Channel 10 and 11 used for conversion
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_10,LL_ADC_SAMPLINGTIME_1CYCLE_5);
	
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_11);
    LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_11,LL_ADC_SAMPLINGTIME_1CYCLE_5);
	
	//start callibration 
	LL_ADC_StartCalibration(ADC1);

	//wait end of calibration
	while (LL_ADC_IsCalibrationOnGoing(ADC1));
	
}

int convert_input(uint32_t channel){
	
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,channel);
    LL_ADC_Enable(ADC1); // lancement de la conversion
    while(!(ADC1->SR & ADC_SR_EOC) ) {} // attente de la fin de conversion
		
    ADC1->SR &= ~ADC_SR_EOC; // validation de la conversion
		
    return ADC1->DR & ~((0x0F) << 12); // retour de la conversion
}

//return 1 -> all good between 456 and 595 (480mV)
//return 0 -> need to fix angle 

int roulisSup40(void) {
	int conversionRes = convert_input(Y);
	int ret = 0;
	//valeur a ne pas depasser cos(40)*595 -> 456
	if  (conversionRes < 456) {
		ret = 0;
	} else {
		ret = 1;
	}
	return ret;

}
