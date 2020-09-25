// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 

static void (* itFonction1)(void);
static void (* itFonction2)(void);
static void (* itFonction3)(void);
static void (* itFonction4)(void);

//Handlers
void TIM1_IRQHandler(void) { 
	TIM1->SR &= ~TIM_SR_UIF;
	itFonction1();
}

void TIM2_IRQHandler(void) { 
	TIM2->SR &= ~TIM_SR_UIF;
	itFonction2();
}

void TIM3_IRQHandler(void) { 
	TIM3->SR &= ~TIM_SR_UIF;
	itFonction3();
	
}

void TIM4_IRQHandler(void) { 
	TIM4->SR &= ~TIM_SR_UIF;
	itFonction4();
}

void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc) {
	//activation de l'horloge
	if (Timer == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	} else if (Timer == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	} else if (Timer == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	} else if (Timer == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	
	//initialiser registre ARR et PSC
	Timer->ARR = Arr;
	Timer->PSC = Psc;	
}

void MyTimer_Start(TIM_TypeDef * Timer){
	Timer->CR1 |= TIM_CR1_CEN ; 
}

void MyTimer_Stop(TIM_TypeDef * Timer){
	Timer->CR1 &= ~(TIM_CR1_CEN);
}

void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio){
	if (Timer == TIM1){
		NVIC->ISER[0] = NVIC->ISER[0] | (1 << TIM1_UP_IRQn);
		NVIC->IP[TIM1_UP_IRQn] = NVIC->IP[TIM1_UP_IRQn] | (Prio << 4);
		itFonction1 = IT_function;
	} else if (Timer == TIM2){
		NVIC->ISER[0] = NVIC->ISER[0] | ( 1<< TIM2_IRQn) ;
		NVIC->IP[TIM2_IRQn] = NVIC->IP[TIM2_IRQn] | (Prio << 4);
		itFonction2 = IT_function;
	} else if (Timer == TIM3){
		NVIC->ISER[0] = NVIC->ISER[0] | ( 1 << TIM3_IRQn) ;
		NVIC->IP[TIM3_IRQn] = NVIC->IP[TIM3_IRQn] | (Prio << 4);
		itFonction3 = IT_function;
	} else if (Timer == TIM4){
		NVIC->ISER[0] = NVIC->ISER[0] | ( 1 << TIM4_IRQn);
		NVIC->IP[TIM4_IRQn] = NVIC->IP[TIM4_IRQn] | (Prio << 4);
		itFonction4 = IT_function;
	}
}


void MyTimer_IT_Enable(TIM_TypeDef * Timer) {
	Timer->DIER |= TIM_DIER_UIE;
}	

void MyTimer_IT_Disable(TIM_TypeDef * Timer) {
	Timer->DIER &= ~TIM_DIER_UIE;
}



