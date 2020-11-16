#ifndef GIROUETTE_H
#define GIROUETTE_H

#include "stm32f1xx_ll_tim.h" 

void Config_Timer_Girouette(TIM_TypeDef *Timer); //TIM3
void start_timer_Girouette(TIM_TypeDef *Timer);
int get_allure_Girouette(TIM_TypeDef *Timer);
void Config_gpio_girouette(void);


#endif
