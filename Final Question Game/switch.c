#include "switch.h"
#include "delay.h"
#include "LED.h" 



void Init_Dpad (void)
{
    //ENABLE PORT(S)
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOGEN;        //GPIO G clock enable

    //CONFIGURE PORT PIN FUNCTIONS
      Dpad_port->MODER&=~ clrbit(0*2) | 
												  clrbit(1*2) | 
													clrbit(2*2) | 
													clrbit(3*2);     //clear pin functions on GPIOB
	
	    Dpad_port->MODER |= 
	                        MASK(2*2) | 
	                        MASK(3*2); 
	
	 	/*onfigure to Pull-up/Pull-down Mode*/
	    Dpad_port->PUPDR &= ~(3u<<(2*CButton)) | 
	                         (3u<<(2*DButton));			

	    Dpad_port->PUPDR |= (2u<<(2*CButton)) | 
	                        (2u<<(2*DButton));	
}


void Init_BB (void)
{
	    //ENABLE PORT(S)
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;        //GPIO B clock enable

    //CONFIGURE PORT PIN FUNCTIONS
      GPIOC->MODER&=~clrbit(13*2);    //clear pin functions on GPIOc
}


