#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_utils.h" 
#include "stm32f1xx_ll_usart.h" 
#include "time.h"
#include "Girouette.h"
#include "Voile.h"
#define ARR1 60000
#define PSC1 24
#define alpha1 1 // 2/3
#define alpha2 1 //
#define beta1 -30 // -30
#define beta2 210 // -180

int angle_voile=45;

void PWM_output_init(TIM_TypeDef *Timer, char Voie)
{
	
	MyTimer_Conf(Timer,ARR1-1,PSC1-1); //Reglage du timer, de son ARR et de son PSC1
	switch(Voie)
	{
		case '1': Timer->CCMR1 &= ~TIM_CCMR1_OC1M_0; // Configuration du mode de la PWM
							Timer->CCMR1 |= TIM_CCMR1_OC1M_1| TIM_CCMR1_OC1M_2;  // Configuration du mode de la PWM
							Timer->CCER |= TIM_CCER_CC1E; // Validation du timer
							break;
							
		case '2': Timer->CCMR1 &= ~TIM_CCMR1_OC2M_0;
							Timer->CCMR1 |= TIM_CCMR1_OC2M_1| TIM_CCMR1_OC2M_2;
							Timer->CCER |= TIM_CCER_CC2E;
							break;
		
		case '3': Timer->CCMR2 &= ~TIM_CCMR1_OC1M_0;
							Timer->CCMR2 |= TIM_CCMR1_OC1M_1| TIM_CCMR1_OC1M_2;
							Timer->CCER |= TIM_CCER_CC3E;
							break;
		
		case '4': Timer->CCMR2 &= ~TIM_CCMR1_OC2M_0;
							Timer->CCMR2 |= TIM_CCMR1_OC2M_1| TIM_CCMR1_OC2M_2;
							Timer->CCER |= TIM_CCER_CC4E;
							break;					
	}
	Timer->CCR1= (ARR1-1)/20; // Réglage de la durée d'impulsion à 5% de la capacité soit 1 ms
}

void config_gpio_pb()
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
	/*RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH &= ~(0xF << 0); //Config du PB08 en alternate push-pull
	GPIOB->CRH |= (0xA << 0);  //Config du PB08 en alternate push-pull*/
}

void start_timer_voile(TIM_TypeDef *Timer)
{
	Timer->CR1 |= TIM_CR1_CEN; // Démarrage du timer
}

int calcul()
{
	int allure= get_allure_Girouette(TIM3); // Récupération de l'allure de la girouette
	if(0<=allure && allure<=45) // Près serré
	{
		return 0; 
	}
	else if(315<=allure && allure<=360) // Près serré
	{
		return 0;
	}
	else if(45<=allure && allure<=180) // Calcul du cap avec la fonction affine
	{
		return 2*allure/3 + beta1;
	}
	else
	{
		return -2*allure/3 + beta2;
	}
}



void Tendre_voile(int angle, TIM_TypeDef *Timer, char Voie)
{
	int angle_conv;
	angle_voile=angle;
	int Ar=ARR1-1;
	angle_conv=(Ar/20)+angle*(Ar/1800); //Passer d'un angle entre 0 et 90 vers une intervalle [1ms;2ms]
	switch(Voie)
	{
		case '1': Timer->CCR1 =angle_conv;
		case '2': Timer->CCR2 =angle_conv;
		case '3': Timer->CCR3 =angle_conv;
		case '4': Timer->CCR4 =angle_conv;
	}
}

int get_angle_voile()
{
	return angle_voile;
}










