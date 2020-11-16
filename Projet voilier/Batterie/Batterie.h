#ifndef BATTERIE_H
#define BATTERIE_H
#include "stm32f1xx_ll_tim.h" 

void configure_gpio_pc2_analog_input(void);
void configure_adc_in12(void);
int convert_single(void);
int maj_etat_batterie(void);
int get_niveau_batterie(void);

#endif

