#include "USART.h"
#include "delay.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


void USARTdelay_us(int time_us)
{    
    unsigned char i;
    while(time_us--)
    {
		for(i=0; i<SystemCoreClock/((int)(4000000.0*11.25)); i++)
			 {
				__nop();
			 }
    }    
}


void USARTdelay_ms(unsigned int time_ms)
{
    unsigned int i;                      
    for(i=0; i<(unsigned int)((SystemCoreClock/180)/100)*time_ms; i++)
			{//count takes 100ms when MCU speed is 180MHz
				__nop();                            
			}
}

void send_usart(unsigned char d)
{
	while(!(USART_MODULE->SR & USART_SR_TC));
	USART_MODULE->DR=d;												//write byte to usart data register
}

void init_USART(void)
{
	SystemCoreClockUpdate();
	unsigned char i1,i2;
		RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;		//usart port clock enable
	
	USART_PORT->MODER&=~(										//clear pin function bits
		(3u<<(2*USART_TX_pin))
		|(3u<<(2*USART_RX_pin))
			);
	USART_PORT->MODER|=(										//reset pin function bits (alternate function)
		(2u<<(2*USART_TX_pin))
		|(2u<<(2*USART_RX_pin))
			);
	
	i1=USART_TX_pin/8;											// used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	i2=USART_RX_pin>>3;											// same as above for RX_pin (uses shift to divide)

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(USART_TX_pin-(i1*8))));    //clear pin function
	USART_PORT->AFR[i1]|=(0x07<<(4*(USART_TX_pin-(i1*8))));			// set usart as alternate function for TX_pin
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(USART_RX_pin-(i2*8))));		// clear pin function
	USART_PORT->AFR[i2]|=(0x07<<(4*(USART_RX_pin-(i2*8))));			//set usart as alternate function for RX_pin
	
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;			//usart clock enable
	USART_MODULE->CR1|=(										//USART CONFIG
			USART_CR1_TE												//transmit enable
			|USART_CR1_RE												//receive enable
			|USART_CR1_UE												//usart main enable bit
			|USART_CR1_RXNEIE										//USART1 receive interrupt enable
			);
			
	NVIC->ISER[1]|=(1u<<7);						//Global NVIC Usart Enable
	USART_MODULE->BRR=0x187;					//set baud rate to 115200
	send_usart(0x1B);

		//this is not an accurate way to set the baudrate
																									//and only works for 16Mhz system clock.
																									
}	
	
	
	


	void myPrintf(const char* format,...)
		{
		char transmit_buffer[255];
		va_list args;
		va_start (args,format);
		vsprintf(transmit_buffer,format,args);
		va_end (args);
		
		int i = 0;
		while(transmit_buffer[i]!='\0'){
		send_usart(transmit_buffer[i]);
		i++;
		}
	}	
		
void cursor_uart(char row,char col)
{
	send_usart(0x1B);					//send_usart esc code
	send_usart(0x5B);					//send_usart esc code
	myPrintf("%i",(int)row);	//send row
	send_usart(';');					//esc code
	myPrintf("%i",(int)col);	//esc code
	send_usart('f');					//send column
}

void USART3_IQR_HANDLER (void)
{
	char received = USART_MODULE->DR;
	USART_MODULE->SR&=~USART_SR_RXNE; //this clears interupt flag in the status register
}



