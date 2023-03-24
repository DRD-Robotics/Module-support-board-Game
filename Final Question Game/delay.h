#ifndef _Delay_H_
#define _Delay_H_
#include <stm32f4xx.h>
/*DEFINES*/

/*FUNCTIONS*/
void Init_Timer2(void); //global whole program delay
void delay_us(unsigned int delay);
void delay_ms(unsigned int delay);
#endif
