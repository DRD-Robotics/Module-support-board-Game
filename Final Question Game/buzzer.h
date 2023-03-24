#ifndef __BUZZER_H
#define __BUZZER_H
#include <stm32f4xx.h>

/*DEFINES*/
#define Buzzer_Pin	13
#define Buzzer_Port GPIOB
#define MASK(x) 		(1UL << (x))

#define Buzz_off			TIM4->ARR=(0) //no sound is produced
#define Buzz_on				TIM4->ARR=(1) //Pitch too high to hear but buzzer is on
#define Note_A	TIM4->ARR =(568)		//Note A5 Freqency :880Hz
#define Note_B	TIM4->ARR =(506)		//Note B5 Freqency :987.77Hz
#define Note_C	TIM4->ARR =(477)		//Note C6 Freqency :1046.5Hz
#define Note_D	TIM4->ARR =(425)		//Note D6 Freqency :1174.7Hz
#define Note_E 	TIM4->ARR =(379)		//Note E6 Freqency :1318.5Hz
#define Note_F	TIM4->ARR =(358)		//Note F6 Freqency :1396.9Hz
#define Note_G	TIM4->ARR =(318)		//Note G6 Freqency :1568Hz

/*FUNCTIONS*/

void Init_Buzzer (void);
void Toggle_Buzzer(void);
void Init_Timer4(void);
void TIM4_IRQHandler(void);	
void victory_screech (void);
void victory_screech2 (void);
void WarmUp_Vocals (void);
void welcome_jingle (void);
void HIGHSCORE (void);
#endif