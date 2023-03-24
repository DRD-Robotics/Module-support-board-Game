#include "delay.h"
#include <stdio.h>
#include <stm32f4xx.h>



/*INITIALIZATION OF TIMERS*/
void Init_Timer2(void)
	{
		RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
		TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																				//APB clock is Fcy/2 = 180MHz/2 = 90MHz
		TIM2->PSC=90-1;                                        //divide by 90 to get 1MHZ
    TIM2->ARR=0xFFFFFF;
		TIM2->CNT=0;												//zero timer counter
		TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
	}
	

	

	
	/* DELAY FUNCTIONS [FOR TIMER 2 ONLY]*/
void delay_us(uint32_t delay)
	{
		uint32_t start = TIM2->CNT;
		uint32_t timeElapsed = TIM2->CNT - start;
		
		do {timeElapsed = TIM2->CNT - start;}
		while (timeElapsed <= delay);
	}

void delay_ms(uint32_t delay)
	{
		delay=delay*1000;
		uint32_t start = TIM2->CNT;
		uint32_t timeElapsed = TIM2->CNT - start;
		
		do {timeElapsed = TIM2->CNT - start;}
		while (timeElapsed <= delay);
	}
