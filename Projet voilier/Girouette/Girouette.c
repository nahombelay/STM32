#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_utils.h" 
#include "stm32f1xx_ll_usart.h" 
#include "stm32f1xx_ll_exti.h"
#include "time.h"
#include "Girouette.h"

int allure = 0;
void Config_Timer_Girouette(TIM_TypeDef *Timer)
{
	MyTimer_Conf(Timer,719,0);
	
	// Mode SMS=001 pour compter sur les fronts des deux voies
	Timer->SMCR &= ~ (TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2);
	Timer->SMCR |=  TIM_SMCR_SMS_0;
	// Config de TI1 
	Timer->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	Timer->CCMR1 |= TIM_CCMR1_CC1S_0;
	Timer->CCER &= ~(TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk | TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
	// Config de TI2
	Timer->CCMR1 &= ~TIM_CCMR1_CC2S_1;
	Timer->CCMR1 |= TIM_CCMR1_CC2S_0;
	Timer->CCER &= ~(TIM_CCER_CC2P_Msk | TIM_CCER_CC2NP_Msk | TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);
	
	
	
	
	
	// Cr�ation de la structure d'initialisation de l'interruption externe
	LL_EXTI_InitTypeDef Struct_interruption;
	
	LL_EXTI_StructInit(&Struct_interruption);
	Struct_interruption.Line_0_31 = LL_EXTI_LINE_5; // Choix du num�ro de la ligne, correspond au Pin 5
	Struct_interruption.Mode = LL_EXTI_MODE_IT; // Choix du mode interruption 
	Struct_interruption.Trigger = LL_EXTI_TRIGGER_RISING;
	Struct_interruption.LineCommand = ENABLE; // Rend active la commande
	LL_EXTI_Init(&Struct_interruption); // Rend actifs les param�tres de la structure d�finie ci-dessus
	
	NVIC->IP[23] =  0x43;
	NVIC->ISER[0] |= 0x01 <<23;
}

void Config_gpio_girouette()
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING);
}

void start_timer_Girouette(TIM_TypeDef *Timer)
{
	Timer->CR1 |= TIM_CR1_CEN;
}

int get_allure_Girouette(TIM_TypeDef *Timer)
{
  return (Timer->CNT)/2;
}

void EXTI9_5_IRQHandler(void) {
	TIM3->CNT=0;
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
}
