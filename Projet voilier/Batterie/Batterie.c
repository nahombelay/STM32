#include "accelero.h"
#include "stm32f1xx_ll_adc.h" 
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h" 
#include "batterie.h"



int niveau_batterie=100;

void init_batterie(void){
	
}
void configure_gpio_pc2_analog_input(){
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG); // On configure la PC2 en entrée analogique
}

void configure_adc_in12(void){
	
	//
	//
	//
	// ************* AVEC LL **************************
	// On active les clock correspondante
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
	// On adapte la fréquence de l'horloge de l'ADC pour ne pas dépasser 14 MHZ
	
	////*************************A NE FAIRE QU'UNE SEULE FOIS ENTRE ICI ET L'ACCELEROMETRE***************************
	// On crée la structure nécessaire à la config
	
		LL_ADC_InitTypeDef ADCinfos;
	
		// Activer le single conversion mode
		ADCinfos.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
	
		// Alignement des données
		ADCinfos.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
		
		
		LL_ADC_Init(ADC1, &ADCinfos);
		
		
		
		//Activer l'ADC
		LL_ADC_Enable(ADC1);
	///****************************************************************************************************************
	//La voie 12 est celle correspondante au PC2
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_12);
	LL_ADC_SetChannelSamplingTime(ADC1,LL_ADC_CHANNEL_12,LL_ADC_SAMPLINGTIME_1CYCLE_5);
	
	//Start Calibration
	LL_ADC_StartCalibration(ADC1);
	
	// Attendre la fin de la calibration
	while(LL_ADC_IsCalibrationOnGoing(ADC1));

}

//a revoir 
int convert_single(){
    ADC1->CR2 |= ADC_CR2_ADON; // Lancement de la conversion
    while(!(ADC1->SR & ADC_SR_EOC)) {}  // Attente de la fin de la conversion
    ADC1->SR &= ~ADC_SR_EOC; // Remise à 0 du flag End Of Conversion
    return ADC1->DR & ~((0x0F) << 12); // Retour du résultat de la conversion
}


// Si la batterie est faible retourne 0 sinon retourne 1
int maj_etat_batterie(void){
		int conversionRes=convert_input(LL_ADC_CHANNEL_12);
		niveau_batterie= (((float) conversionRes)/4095.0)*100.0;
		int res=0;
		// SEUIL DE 20% DE BATTERIE EST 4095/5 SOIT 819
		if (conversionRes < 819){
			res=0;
		}
		else {
			res=1;
		}
		return res;	
}

int get_niveau_batterie(void) {
	return niveau_batterie;
}
