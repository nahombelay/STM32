/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_tim.h"
#include "MyTimer.h"


#include "Chrono.h"
#include "recepteurRF.h"
#include "moteurCC.h"
#include "accelero.h"
#include "communication.h"
#include "batterie.h" 
#include "voile.h"
#include "girouette.h"





void  SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

void fonction_int(void){
	
	if (roulisSup40() == 0) {
		Tendre_voile(0, TIM4, '3');
	}
	
	Tendre_voile(calcul(), TIM4, '3');
	tournerTable();
	//UART plus tard
	
}
void Configuration(void){
	recepteurRFConf();
	moteurccConf();
	confAccelero();
	configure_uart();
	configure_gpio_pc2_analog_input();
	config_gpio_pb();
	//start_timer_voile(TIM4);
	//TIM4->CR1 |= TIM_CR1_CEN; 
	
	configure_adc_in12();
	
	Config_Timer_Girouette(TIM3);
	Config_gpio_girouette();
	start_timer_Girouette(TIM3);
	
	PWM_output_init(TIM4,'3');
	
	//Configuration du timer de d�bordement
	Chrono_Conf(TIM1, fonction_int);
	
	
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	

int main(void)
{
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Add your application code here */
  
	//int sup40;
	
	Configuration();
	
	send_battery();
	

  /* Infinite loop */
  while (1)
  {
	  //sup40 = roulisSup40();
	  //tournerTable(getPWMmoteur(), getSens());
	  //test simu tournertable(PA6, 1) 
	  //sur PA6 on output un pwm TIM3, enable tim3 et gpioA, PA6 output psuh pull
	  
	  
	  
	  
  }
}







/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucl�o*********************************
  //LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
