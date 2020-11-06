// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_rcc.h"

void Chrono_Conf_io(void);
int etatChrono = 1; // Si etat = 0 -> chorno pas d�mar�; Si chrono = 1 -> en marche
int etatLed = 0; //meme principe

// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);

/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	//Appel de chronoConfio
	Chrono_Conf_io();
	
	
	// R�glage Timer pour un d�bordement � 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	
}


/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 Hund, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		if (etatLed == 0) {
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_10);
		} else {
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_10);
		}
		etatLed = (etatLed + 1) % 2 ; 
		
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	
}


/**
	* @brief  polle les 3 entr�es et g�n�re les actions � faire
  * @note   Fct � lancer en t�che de fond (non bloquante)
	* @param  None 
  * @retval None
  */
void Chrono_Background(void) {
	
	//Revoir gestion des bouttons pour trouver le moment ou on relache le boutton 
	
	if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_13)) {
		Chrono_Reset();
		etatChrono = 0;
	} else if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_8)) {
		if (etatChrono == 1) {
			Chrono_Start();
			etatChrono = (etatChrono + 1) % 2;
		} else {
			Chrono_Stop();	
			etatChrono = (etatChrono + 1) % 2;
		}
	}
		
	
}

/**
* Chrono_Conf_io( ): elle doit permettre de configurer les 3 IO 
* pour recevoir les 2 BP et la LED. Elle est priv�e
* et doit �tre appel�e par Chrono_Conf( ).
*/

void Chrono_Conf_io(void) {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	//(S&S : PC8)
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_FLOATING);
	
	//(Reset : PC13)
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_FLOATING);
	
	//Led (PC10)
	//Push-Pull
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT_10MHz);
	LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
	
	//Open Drain
	//LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_OPENDRAIN);
	
}

void Conf_USART(USART_TypeDef * USART) {
	//init la clock en fonction de la valeur de usart
	LL_USART_InitTypeDef Usart_struct;
	
	if (USART == USART1) {
		LL_APB1_GRP1_EnableClock(RCC_APB2ENR_USART1EN);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
	} else if (USART == USART2) {
		LL_APB1_GRP1_EnableClock(RCC_APB1ENR_USART2EN);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
	} else if (USART == USART3) {
		LL_APB2_GRP1_EnableClock(RCC_APB1ENR_USART3EN);
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
	}
	
	LL_USART_StructInit(&Usart_struct);
	Usart_struct.TransferDirection = LL_USART_DIRECTION_TX;
	
	LL_USART_Init(USART, &Usart_struct);
	LL_USART_Enable(USART);
}











