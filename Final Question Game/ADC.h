#ifndef _ADC_H
#define _ADC_H
#include <stm32f4xx.h>

/*DEFINES*/
#define ADC_input_port		GPIOC
#define ADC_input_pin			0
#define ADC_Channel				10





/*FUNCTIONS*/
void init_ADC(void);
unsigned short read_adc(void);
#endif 
