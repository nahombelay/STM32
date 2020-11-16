#include <stdio.h>
#include <stdlib.h>

#include "communication.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_usart.h" 

#include "batterie.h" 
#include "accelero.h"
#include "Voile.h"


void configure_uart(void){
	//configure PA9 (openclassroom)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9);
	
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // validation horloge USART1
    USART1->CR1 |= USART_CR1_UE; // Activation de l'USART
    USART1->CR1 &= ~USART_CR1_M; // Choix d'une taille de 8 bits de données
    USART1->CR2 &= USART_CR2_STOP; // Choix d'un seul bit de stop
    USART1->BRR |= 468 << 4; // Fixe le baud rate à 9600bps
    USART1->BRR |= 75; // Fixe le baud rate à 9600bps
    USART1->CR1 |= USART_CR1_TE; // Envoi de la première trame d'attente
	
	
}

void send(char data){
    USART1->DR |= data; // Ecriture de la donnée dans le registre DR
    while(!(USART1->SR & USART_SR_TC)) {} // Attente de la fin de transmission
}


//revoir: logic analyzer renvoit 0 tt le temps
void send_battery(void){
	char battery [4];
	int batteryInt = get_niveau_batterie();
	sprintf(battery, "%d", batteryInt);	
	
	int i = 0;
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	
	while (battery[i] != '\0'){
		send(battery[i]);
		i++;
	}
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
}

void send_roulis(){
	char * roulisNotOK = "Roulis : need fixing";
	int detection = roulisSup40();
	
	
	if (detection == 0) {
		int i = 0;
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
		LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	
		while (roulisNotOK[i] != '\0'){
			send(roulisNotOK[i]);
			i++;
		}
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
	}
}

void send_angle(){
	int angle = get_angle_voile();
	char angleChar [4];
	
	sprintf(angleChar, "%d", angle);	
	
	int i = 0;
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
	
	while (angleChar[i] != '\0'){
		send(angleChar[i]);
		i++;
	}
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
		
}




