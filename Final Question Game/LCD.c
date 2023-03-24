#include "LCD.h"
#include "delay.h"
#include "buzzer.h"
#include <stdio.h>
#include <string.h>
#include "switch.h"
void lcd_Tim3_us(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;	//ENABLE TIMER 3
	TIM3->PSC=9000-1;									//COUNTER RELOAD VALUE. PERIOD OF 100ms WHEN F_APB = 90MHz & PSC = 256
	TIM3->ARR=5000;	
	TIM3->CNT =0;											//COUNTER SET TO 0
	TIM3->CR1|=TIM_CR1_CEN;						//START CNT
}

void lcd_Tim3_wait(unsigned int delay_us)
{
	unsigned int tick = TIM3->CNT;
	while((TIM3->CNT-tick)<delay_us)
	{
		__NOP();
	}
}

void lcd_delayus (unsigned int us)
{
	unsigned char i;
	while (us--)
	{
		for(i=0; i<SystemCoreClock/4000000; i++)
		{
		__NOP();
	
		}
	}
}


void WaitLcdBusy(void)
{	
	unsigned int port = LCD_PORT->IDR;
	set_LCD_bus_input();
	set_LCD_RW();								//read command
	clr_LCD_RS();								//Instruction command
	set_LCD_E();								//enable command
	port|= LCD_PORT->IDR; 			//reads lcd port
	port&= MASK(LCD_D7_pin); 		//masks busy bit 7		
	
	do
  {
  	set_LCD_E();							//start of pulse
		lcd_delayus(10);  				//gives the LCD some time to reply
		port=LCD_PORT->IDR;				//Reads port
		port&= MASK(LCD_D7_pin);	//masks bit 7
		clr_LCD_E();							//end of pulse
  	lcd_delayus(10);					//gives the Lcd some time to reply
  } while (port!=0); 					//read busy flag again and again till it becomes 0
  set_LCD_bus_output();				//Configure data bus lines as digital outputs
}

void set_LCD_data(unsigned char d)
{
	LCD_PORT->BSRR=(0xff<<(LCD_D0_pin+16));	//clear data lines
	LCD_PORT->BSRR=(d<<LCD_D0_pin);					//put data on lines
}

//10us high pulse on LCD enable line//
void LCD_strobe(void)		
{
	lcd_delayus(10);
	set_LCD_E();
	lcd_delayus(10);
	clr_LCD_E();
}


//sends a byte to the LCD control register//
void SendCommandLCD(unsigned char cmd)		
{
					////Initialise LCD////
	WaitLcdBusy();				//wait for LCD to be not busy
	clr_LCD_RS();					//clear control command
	clr_LCD_RW();					//clear write command

						//////4-bit////////
				/////// UPPER NIBBLE ///////
	/*set data on bus --- extracting upper nibble*/
	set_LCD_data(cmd & 0xF0);
	/*apply command*/
	LCD_strobe();	

			//////// LOWER NIBBLE ///////	
	/*set data on bus --- shifing lower nibble into place*/
	set_LCD_data((cmd & 0x0F)<< 4);	
	/*apply command*/
	LCD_strobe();					
}

//sends a char to the LCD display//
void SendCharLCD(unsigned char put)	
	{
								//////4-bit////////
				/////// UPPER NIBBLE ///////
		WaitLcdBusy();							//wait for LCD to be not busy
		set_LCD_RS();								//clear control command
		clr_LCD_RW();								//clear write command
		set_LCD_data(put);		//set data on bus
		LCD_strobe();								//apply command
		
		
			  //////// LOWER NIBBLE ///////			
		/*set data on bus --- '0x0F' base line for where to put first nibble*/
		set_LCD_data((put)<< 4);		
		LCD_strobe();					//apply command
	} 


//"char*" points to the memory location of a sequence of multiple chars//
void LCD_string (char* string) 
	{
		int i=0;
		for(i=0; string[i] !='\0'; i++)
		{
			SendCharLCD(string[i]);
		}
		
//		while(*string > 0)
//			{
//				SendCharLCD(*string++);
//			}

	}

void loadLine1 (void)
			{
				LCD_LINE1;
				lcd_delayus(1000);
			}
	void loadLine2 (void)
			{
				LCD_LINE2;
				lcd_delayus(1000);
			}		
			
void LCDclrAll(void)
{
	WaitLcdBusy();
	LCD_LINE1;
	LCD_CLR();
	WaitLcdBusy();
	LCD_LINE2;
	LCD_CLR();
}
void initLCD(void)
	{
		
			SystemCoreClockUpdate();
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;	//enable LCD port clock
		
		
			///CONFIG LCD GPIO PINS///
		
			/*clear pin direction settings*/
			LCD_PORT->MODER&=~(	

				(3u<<(2*LCD_RS_pin))			//command
				|(3u<<(2*LCD_RW_pin))  		//write
				|(3u<<(2*LCD_E_pin))			//clr enable
	//		|(0xffff<<(2*LCD_D0_pin))	//clr PIN MODE (8bit)	
				|(0xff00<<(2*LCD_D0_pin))	//clr PIN MODE (4bit)
				);
		
			/*set pin direction settings to digital outputs*/
			LCD_PORT->MODER|=(
		
				(1u<<(2*LCD_RS_pin))
				|(1u<<(2*LCD_RW_pin))
				|(1u<<(2*LCD_E_pin))
	//		|(0x5555<<(2*LCD_D0_pin)) //SET PIN OUTPUT MODE (8bit)
				|(0x5500<<(2*LCD_D0_pin)) //SET PIN OUTPUT MODE (4bit)
			)
		;
		
			/*set all lines to low by default*/
			clr_LCD_RS();					
			clr_LCD_RW();
			clr_LCD_E();
			
			lcd_Tim3_wait(40000);		//40ms startup delay
			set_LCD_RS();          	//text command
			clr_LCD_RW();          	//write command
			set_LCD_data(0x30);    	//set data on bus
			LCD_strobe();          	//apply command
			lcd_Tim3_wait(40);
			
			SendCommandLCD(0x28);   //Function set: 2 Line, 4-bit, 5x7 dots

			SendCommandLCD(0x0F);   //Display on, Cursor blinking command
			lcd_delayus(40);       	//40 microsecond delay
			SendCommandLCD(0x01);   //Clear LCD
			/* 1.6 millisecond delay [HAS TO BE ABOVE 1.53MS--FROM PP SLIDES] */
			lcd_delayus(1600);     
			/* Entry mode, auto increment with no shift --- move right after every char is inputted */										  
			SendCommandLCD(0x06);
	}


	
	void custom_char(unsigned char location, unsigned char *data)
{
	uint8_t i;
	if(location<8)
	{
		SendCommandLCD((0x40 + (8*location))); /* CGRAM location */
		for(i=0;i<8;i++)
		SendCharLCD(data[i]); /* Data to be stored at CGRAM location */
	}
}
	

void sadchar (void)
	{
		unsigned char bell_char[8] = {0x00,0x0A,0x04,0x04,0x00,0x0E,0x11}; 
		custom_char(4,bell_char);
		SendCommandLCD(0xC6);
		SendCharLCD(4);
		return;
	}


void only_scroll (unsigned char questlen)
{
    int loop;
    for(loop=0; loop<questlen; loop++)
  {
    SendCommandLCD(0x18);
    delay_ms(300);
  }
}


	
	
	void scroll2 (void)
{
		unsigned int Loopcnt=0; //dictates how many times to loop
		unsigned int txtLen=0;	//variable length of text
		char *text = "  THANKS FOR PLAYING!!";
		txtLen = strlen(text);	//calcs the amount of chars in a string
		
		while(1)		//ENTER 'FOREVER' LOOP - THIS LOOP WILL NEVER EXIT
			{
				SendCommandLCD(0x8f); //last positon on screen
				SendCommandLCD(0x83); //screen pos
				LCD_string(text);

				for(Loopcnt=0; Loopcnt < txtLen; Loopcnt++)
						{
							SendCommandLCD(0x18);
							lcd_delayus(150000);		
						}	
			}
	}
	


void correct_flash (void)
	{
			victory_screech();
			LCD_CLR();
			LCD_LINE1;
			LCD_string("CORRECT!!");
     
		
	}
	
void incorrect_flash (void)
	{
			victory_screech();
			LCD_CLR();
			LCD_LINE1;
			LCD_string("WRONG!");
      
		
	}
	
	
	