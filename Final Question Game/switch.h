#ifndef __SWITCH_H
#define __SWITCH_H
#include "stm32f4xx.h"

/*DEFINES*/
#define Dpad_port		GPIOG  
#define BB_Port			GPIOC
#define AButton	0
#define BButton 1
#define CButton 2
#define DButton 3

#define clrbit(x) (3UL << (x))
#define MASK(x) (1UL << (x))
#define pupMASK(x)  (2UL << (x)) 

#define UserButtonPress  BB_Port->IDR & MASK(13)
#define AButtonPress  	(Dpad_port->IDR & MASK(0))
#define BButtonPress  	(Dpad_port->IDR & MASK(1))

#define CButtonPress  	(Dpad_port->IDR & MASK(2))
#define DButtonPress  	((Dpad_port->IDR &(1u<<(DButton)))>>DButton)



/*FUNCTIONS*/
void Init_Dpad (void);
void Init_BB (void);
_Bool CorDButton(int pin);
#endif