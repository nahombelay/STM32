#ifndef VOILE_H
#define VOILE_H

#include "stm32f1xx_ll_tim.h" 

void PWM_output_init(TIM_TypeDef *Timer, char Voie);
void config_gpio_pb(void);
void start_timer_voile(TIM_TypeDef *Timer);
int calcul(void);
void Tendre_voile(int angle, TIM_TypeDef *Timer, char Voie);
int get_angle_voile(void);



#endif
