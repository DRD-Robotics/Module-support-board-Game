#include "LED.h"
#include "delay.h"

void OnBoard_LED_INIT (void)
{
	//ENABLE PORT(S)
	RCC->AHB1ENR|= RCC_AHB1ENR_GPIOBEN;	//ONLY GPIO B clock enable
	
	//CONFIGURE PORT PIN FUNCTIONS
	GPIOB->MODER&=~(3u<<(0*2))|
								 (3u<<(7*2))|
								 (3u<<(14*2));						//clear pin functions on GPIOB
	
	
	GPIOB->MODER|=(1u<<(0*2))|
								(1u<<(7*2))|
								(1u<<(14*2));						//set new pin functions on GPIOB
}

void Init_OD_LEDs(void)
{
	/*ENABLE PORT(S)*/
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;    //GPIO C clock enable for push-pull and open drain LEDs
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOFEN;		//GPIO F clock enable for White LED
	
	
	/*CONFIGURE PORT PIN FUNCTIONS*/
	OD_Port->MODER &=~
	clrpin(Red_OD*2) |
	clrpin(Ylw_OD*2) |
	clrpin(Grn_OD*2);	 										//clear pin functions on GPIOC
	
	White_OD_Port->MODER &=~
	clrpin(White_OD*2);                   //clear pin functions on GPIOF
	
	OD_Port->MODER |=	
	MASK(Red_OD*2) |
	MASK(Ylw_OD*2) |
	MASK(Grn_OD*2);			                  //set new pin functions on GPIOC
	
	White_OD_Port->MODER |=	
	MASK(White_OD*2);	                   //set new pin functions on GPIOF
	
	/*CONFIGURE TO OPEN DRAIN MODE*/
	OD_Port->OTYPER|=	
	MASK(Red_OD) |
	MASK(Ylw_OD) |	
	MASK(Grn_OD);

	White_OD_Port->OTYPER|= MASK(White_OD);
}	

void All_ODs_Off (void)
{
	White_OD_Port->BSRR= MASK(White_OD);
	OD_Port->BSRR= MASK(Red_OD)|MASK(Ylw_OD)|MASK(Grn_OD);
}

void Ready_Strobe (void)
{
	
	/*Red OD strobe*/
	OD_Port->BSRR= ODRedOn| MASK(Ylw_OD)|MASK(Grn_OD);
	delay_ms(200);
	OD_Port->BSRR=ODRedOff;
	delay_ms(200);
	OD_Port->BSRR=ODRedOn;
	delay_ms(200);
	OD_Port->BSRR=ODRedOff;
	
	delay_ms(2000);
}



void Steady_Strobe (void)
{
	/*Red and Yellow OD strobe*/
	OD_Port->BSRR=ODYlwOn;
	delay_ms(200);
	OD_Port->BSRR=ODYlwOff;
	delay_ms(200);
	OD_Port->BSRR=ODYlwOn;
	delay_ms(200);
	OD_Port->BSRR=ODYlwOff;
  delay_ms(2000);
}

void Go_Strobe (void)
{
	/*All 3 strobe*/		
	OD_Port->BSRR = ODRedOn| ODYlwOn| ODGrnOn;
	delay_ms(100);
	OD_Port->BSRR=ODRedOff| ODYlwOff|ODGrnOff;
	delay_ms(100);
	OD_Port->BSRR = ODRedOn| ODYlwOn| ODGrnOn;
	delay_ms(100);
	OD_Port->BSRR=ODRedOff| ODYlwOff|ODGrnOff;
	delay_ms(100);
	OD_Port->BSRR = ODRedOn| ODYlwOn| ODGrnOn;
	delay_ms(100);
	OD_Port->BSRR=ODRedOff| ODYlwOff|ODGrnOff;
	
}


void Init_LED_Digits(void)
{
  //ENABLE GPIOE//
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	
	//clear pin functions on GPIOE
	LED_Digits_PORT->MODER&=~ 
	(3u<<(2*2))|
	(3u<<(2*3))|
	(3u<<(2*4))|
	(3u<<(2*5))|
	(3u<<(2*6))|
	(3u<<(2*7))|
	(3u<<(2*8))|
	(3u<<(2*9))|
	(3u<<(2*10))| 
	(3u<<(2*11))|
	(3u<<(2*15));		
	
	//Set pin functions to output mode
	LED_Digits_PORT->MODER|= 
	(1u<<(2*2))| 
	(1u<<(2*3))|
	(1u<<(2*4))|
	(1u<<(2*5))|
	(1u<<(2*6))|
	(1u<<(2*7))|
	(1u<<(2*8))|
	(1u<<(2*9))|
	(1u<<(2*10))| 
	(1u<<(2*11))| 
	(1u<<(2*15));
	}

    /*only left side connected*/	
		void Active_LE1(void)
	{
		LED_Digits_PORT->BSRR=(1u<<(10)) ;
		LED_Digits_PORT->BSRR=(1u<<(15+16)|(1u<<(11+16))) ; //OFF
	}
	
		/*only right side connected*/
		void Active_LE2(void)
	{
		LED_Digits_PORT->BSRR=(1u<<(11)) ;
		LED_Digits_PORT->BSRR=(1u<<(15+16)|(1u<<(10+16))) ; //OFF
		
	}
	
	/*both sides connected*/
	void Active_both (void)
	{
		LED_Digits_PORT->BSRR=(1u<<(11)|(1u<<(10))) ;
		LED_Digits_PORT->BSRR=(1u<<(15+16)) ; //OFF
	}
	
	
	void CLR_Bits (void)
	{
		LED_Digits_PORT->BSRR=
	(1u<<(2+16))| 
	(1u<<(3+16))|
	(1u<<(4+16))|
	(1u<<(5+16))|
	(1u<<(6+16))|
	(1u<<(7+16))|
	(1u<<(8+16))|
	(1u<<(9+16))|
	(1u<<(10))|
	(1u<<(11));
	}	
void CLR_Digit (void)
{
	LED_Digits_PORT->BSRR=
	(1u<<(2+16))| 
	(1u<<(3+16))|
	(1u<<(4+16))|
	(1u<<(5+16))|
	(1u<<(6+16))|
	(1u<<(7+16))|
	(1u<<(8+16))|
	(1u<<(9+16));
}	

void zero (void)
{
	  CLR_Bits(); 									//set all pins to off
		Active_both();								//activate both Left power and right power
    LED_Digits_PORT->BSRR=D0;			//display 0 on both
		delay_ms(500);							  //wait	
}

	
	