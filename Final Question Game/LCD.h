#ifndef _LCD_H_
#define _LCD_H_
#include <stm32f4xx.h>
/*DEFINES*/
#define MASK(x) (1UL << (x))

//LCD pins///
#define LCD_PORT	GPIOD
#define LCD_RS_pin	11
#define LCD_RW_pin	12
#define LCD_E_pin		13
#define LCD_D0_pin	0
#define LCD_D1_pin	1
#define LCD_D2_pin	2
#define LCD_D3_pin	3
#define LCD_D4_pin	4
#define LCD_D5_pin	5
#define LCD_D6_pin	6

//Busy Bit//
#define LCD_D7_pin	7

//LCD SCREEN//
#define LCD_LINE1		SendCommandLCD(0x80)
#define LCD_LINE2		SendCommandLCD(0xC0)
#define LCD_CLR()		SendCommandLCD(0x01)

//MACROS//
#define set_LCD_RS()	LCD_PORT->BSRR=(1u<<LCD_RS_pin)
#define clr_LCD_RS()	LCD_PORT->BSRR=(1u<<(LCD_RS_pin+16))
#define set_LCD_RW()	LCD_PORT->BSRR=(1u<<LCD_RW_pin)
#define clr_LCD_RW()	LCD_PORT->BSRR=(1u<<(LCD_RW_pin+16))
#define set_LCD_E()		LCD_PORT->BSRR=(1u<<LCD_E_pin)
#define clr_LCD_E()		LCD_PORT->BSRR=(1u<<(LCD_E_pin+16))





#define set_LCD_bus_input()		LCD_PORT->MODER&=~(0xffff<<(2*LCD_D0_pin))
#define set_LCD_bus_output()	LCD_PORT->MODER|=(0x5555<<(2*LCD_D0_pin))


#define set_LCD_bus_input_4_bit()		LCD_PORT->MODER&=~(0xff00<<(2*LCD_D0_pin))
#define set_LCD_bus_output_4_bit()	LCD_PORT->MODER|=(0x5500<<(2*LCD_D0_pin))

#define Q1  (" The moon is made of cheese")

/*FUNCTIONS*/
//void lcd_delay_us(unsigned int us);
void WaitLcdBusy(void);
void set_LCD_data(unsigned char d);
void LCD_strobe(void);
void initLCD(void);
void LCD_string(char* string); 


//new
void SendCharLCD(unsigned char put);
void SendCommandLCD(unsigned char cmd);
void lcd_Tim3_us(void);
void lcd_Tim3_wait(unsigned int delay_us);
void lcd_delayus (unsigned int us);
//void WaitLcdBusy2(void);
void LCD_Custom_Char (unsigned char CharLen, unsigned char *msg);
void sadchar (void);
void only_scroll (unsigned char questlen);
void scroll (char* text);
void scroll2 (void);
void welcome_text (void);
void correct_flash (void);
void incorrect_flash (void);
void loadLine1 (void);
void loadLine2 (void);
void LCDclrAll (void);
void custom_char(unsigned char location, unsigned char *data);
#endif