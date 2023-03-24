#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"
#define MASK(x) (1UL << (x))
#define clrpin(x) (3UL<<(x))

/*DEFINES*/
#define On_Board_LED_Port GPIOB
#define grnLED 0


//OPEN DRAIN
#define OD_Port GPIOC
#define PP_Port GPIOC
#define Red_OD 7
#define Ylw_OD 8
#define Grn_OD 9

#define White_OD_Port GPIOF
#define White_OD 10


#define LED_Digits_PORT	GPIOE
#define	D0	0x1EC
#define D1	0x180
#define D2	0xF8
#define	D3	0x1F0
#define	D4	0x194
#define	D5	0x174
#define	D6	0x17C
#define	D7	0x1C0
#define	D8	0x1FC
#define	D9	0x1D4

#define ODRedOn        (1u<<(Red_OD))
#define ODRedOff       (1u<<(Red_OD+16))

#define ODGrnOn        (1u<<(Grn_OD))
#define ODGrnOff       (1u<<(Grn_OD+16))

#define ODYlwOn        (1u<<(Ylw_OD))
#define ODYlwOff       (1u<<(Ylw_OD+16))

#define ODWhiteOn      (1u<<(White_OD))
#define ODWhiteOff     (1u<<(White_OD+16))



#define OE_low() GPIOE->ODR&=~(1u<<(15))

/*FUNCTIONS*/

////	ONBOARD LEDS	////
void OnBoard_LED_INIT (void);
void LED_ON (void);
void LED_OFF (void);
void Init_Timer5 (void);
void FiveHz_green (void);
void TIM5_IQRHandler(void);

////	OPEN DRAIN/PUSH-PULL LEDS ////
void Init_OD_LEDs(void);
void Ready_Strobe (void);
void Steady_Strobe (void);
void Go_Strobe (void);
void All_ODs_Off (void);

////	 LED DIGITS		////
void Init_LED_Digits(void);
void count_up (void);
void Active_LE1(void);
void Active_LE2(void);
void Active_both (void);
void All_ON_LED (void);
void Init_Timer2(void);
void Units (void);
void CLR_Bits (void);
void CLR_Digit (void);
void zero (void);
void one(void);

#endif