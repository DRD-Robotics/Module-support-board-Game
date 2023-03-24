#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>

/*DEFINES*/
#define USART_MODULE	USART3
#define USART_PORT		GPIOD
#define USART_TX_pin	8
#define USART_RX_pin	9
#define BAUDRATE			9600

/*FUNCTIONS*/
void send_usart(unsigned char d);
void init_USART(void);
void myPrintf(const char* format,...);
void cursor_uart(char row,char col);
void USART3_IQR_HANDLER (void);
void USARTdelay_us(int time_us);
void USARTdelay_ms(unsigned int time_ms);

#endif
