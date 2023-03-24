#include "buzzer.h"
#include "delay.h"


void Init_Timer4(void)
	{
		RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;		//timer 4 clock enabled
		TIM4->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																				//APB clock is at 180MHz
		TIM4->PSC= 90-1;										//90Mhz/90=1MHZ
		TIM4->ARR = 1;											//ARR ctrls frequency of buzzer
		NVIC->ISER[0]|=(1u<<30);						//timer 4 global interrupt
		TIM4->CNT=0;												//timer counter
		TIM4->CR1|=TIM_CR1_CEN;							//start timer counter
	}
	
void TIM4_IRQHandler(void)			//TIMER 4 INTERRUPT HANDLER
	{
		TIM4->SR&=~TIM_SR_UIF;				//clear interrupt flag in status register
		Toggle_Buzzer();							//Toggles the Buzzer 
	}

	
void Init_Buzzer (void)
	{
		//ENABLE PORT(S)
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;		//GPIO B clock enable
		
		//CONFIGURE PORT PIN FUNCTIONS
		GPIOB->MODER&=~(3u<<(2*Buzzer_Pin));	//clear pin functions on GPIOB
		GPIOB->MODER|=(1u<<(2*Buzzer_Pin));		//set new pin functions on GPIOB
	}

void Toggle_Buzzer(void)
	{
		unsigned int port = Buzzer_Port->ODR;				
		port&=MASK(Buzzer_Pin);											// masks buzzer pin in the ODR Register
		if (port != 0)															// if odr is outputting...
		{
			Buzzer_Port->BSRR = MASK(Buzzer_Pin)<<16;	//...turn off buzzer pin ....
		}
		else
		{	
			Buzzer_Port->BSRR = MASK(Buzzer_Pin);			//...otherwise buzzer pin is on
		}
	}

	void WarmUp_Vocals (void)											//gets buzzer ready to preform more smoothly
	{
	  delay_ms(10);
		Buzz_off;
		delay_ms(10);
		Buzz_on;
		delay_ms(10);
	}

	
	void welcome_jingle (void)									//start up jingle
	{
		Note_E;
		delay_ms(200);
		Note_E;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_A;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_A;
		delay_ms(200);
		Note_C;
		delay_ms(200);
		Note_G;
		delay_ms(200);
		Note_B;
		delay_ms(200);
		Note_F;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_C;
		Note_E;
		delay_ms(200);
		Note_E;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_A;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_A;
		delay_ms(200);
		Note_C;
		delay_ms(200);
		Note_G;
		delay_ms(200);
		Note_B;
		delay_ms(200);
		Note_F;
		delay_ms(200);
		Note_D;
		delay_ms(200);
		Note_C;
		Buzz_off;
		return;
	}
		
	void HIGHSCORE (void)
	{
		Note_A;
		delay_ms(200);
		Note_C;
		delay_ms(200);
		Note_E;
		Buzz_off;
		return;
	}
	
	void victory_screech (void)  //correct answer noise
	{
			
				Note_C;
				delay_ms(200);
				Note_D;
				delay_ms(200);
				Note_C;
				delay_ms(200);
				Note_D;
				delay_ms(200);
				Buzz_off;
				return;
	}
	
void victory_screech2 (void)  //wrong answer noise
{
			
				Note_B;
				delay_ms(200);
				Note_A;
				delay_ms(200);
				Note_B;
				delay_ms(200);
				Note_A;
				delay_ms(200);
				Buzz_off;
	      return;
}










