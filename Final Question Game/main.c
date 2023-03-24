#include <stm32f4xx.h>
#include <string.h>
#include "PLL_Config.c"
#include "delay.h"
#include "LED.h"
#include "LCD.h"
#include "switch.h"
#include "buzzer.h"
#include "USART.h"
#include "DAC.h"
#include "ADC.h"




int main (void)
{
		int count = 0; //STORES WHAT PART OF GAME PLAYER IS AT
		int score = 0;
		int i=0;
		unsigned int LED_Num [10] = {D0,D1,D2,D3,D4,D5,D6,D7,D8,D9};	//HEX FOR LED DIGITS 1-9 
		int	level =0;  //STORES THE DIFFICULTY LEVEL THE PLAYER WANTS TO PLAY
		PLL_Config(); //180 MHz 
		SystemCoreClockUpdate(); //UPDATE SYSTEM CORE CLOCK
		/*INITIALISE ALL SUPPORT BOARD FEATURES AND TIMERS*/
		OnBoard_LED_INIT();   
		Init_LED_Digits();
		lcd_Tim3_us();		
		initLCD();
		Init_Timer2();
		Init_Timer4();
    Init_Buzzer();
		Init_Dpad();
		Init_OD_LEDs();
		Init_BB();
		LCD_CLR();
		WarmUp_Vocals();
		
			while (1)
		{
     All_ODs_Off();   //TURN OFF BLINDING OPEN DRAIN LIGHTS
			switch (count)  //START GAME SEQUENCE
      {
				case 0:	
					score=0;
				  level=0;
          loadLine1();              //lcd is slow so needed for it to catch up. PRINTS ON LINE 1 OF LCD
					LCD_string ("Hold Blue");				  
					loadLine2();              //PRINTS ON LINE 2 OF LCD
					LCD_string ("Button To Start!");
				  delay_ms(1000);          //1s DELAY TO REDUCE SCREEN FLICKER DUE TO REFRESH RATE
					if (UserButtonPress)
					{	
						LCDclrAll();           //CLEARS BOTH LINES OF LCD SCREEN
						loadLine1();
						LCD_string ("Ready...");
						Ready_Strobe();       //ONLY RED LIGHT STROBES
						delay_ms(2000);
						
						LCDclrAll();
						loadLine1();
						LCD_string ("Steady...");
						Steady_Strobe();      //ONLY YELLOW LIGHT STROBES
						delay_ms(2000);
					
						LCDclrAll();
						loadLine1();
						LCD_string ("Go!!!");
						Go_Strobe();         //ALL LIGHTS STROBE
						delay_ms(2000);
						count = 1;          //GO TO OPENING SCREEN
					}						
      	break;
				
      	case 1:
					LCDclrAll();
					loadLine1();
					LCD_string ("WELCOME TO THE");
					loadLine2();
					LCD_string ("ONLY QUESTION!");
				  welcome_jingle();
          delay_ms(3000);
				
					
				
					Active_LE2();								        //turn on left side only (and leave whats on right side uncleared)
					CLR_Digit();								        //clear last digit in memory
					LED_Digits_PORT->BSRR=LED_Num[0]; //set array pointer to display 0
					LCDclrAll();
					
					count=11;                           //GO TO SET DIFFICULTY SCREEN
			    delay_ms(500);
      	break;
				
				case 11:
					LCDclrAll();
					loadLine1();
				  LCD_string("Set Level: A:Up");
					loadLine2();
				  LCD_string("B:reset Blu:entr");
				  delay_ms(1000);
				  if (AButtonPress)                      //WHEN USER PRESSES THE A BUTTON...
						{
						 LCDclrAll();
						 Active_LE2();								         //...turn on left side only (and leave whats on right side uncleared)
		         CLR_Digit();								          //clear last digit in memory
		         LED_Digits_PORT->BSRR=LED_Num[i++];  //GO TO NEXT NUMBER IN LED DIGIT ARRAY 
						 level++;                             //add 1 to DIFICULTY level
						 delay_ms(500);
						 count = 11;													//RETURN TO DIFFICULTY SCREEN AND AWAIT INSTRUCTION
						}
					if (BButtonPress)												//WHEN USER PRESSES THE B BUTTON...
						{
							LCDclrAll();												//EVERYTHING WILL CLEAR AND GAME WILL RESET
							level=0;
							count = 0;
						}
					if (UserButtonPress)                   //WHEN USER PRESSES THE BLUE USER BUTTON...   
					{
						LCDclrAll();
						count=2;                             //...GAME LEVEL IS CONFIRMED AND GAME WILL START
					}
					break;
				
				
				
      	case 2:
					SendCommandLCD(0x83);                  //PRINTS TEXT IN CENTRE OF LCD SCREEN
					lcd_delayus(100);
					LCD_string ("Question 1!");
					SendCommandLCD(0xC3);
					lcd_delayus(100);
					LCD_string ("(Press blue)");
					delay_ms(500);
					do                                   //DEPENDING ON WHAT DIFICULTY THE PLAYER CHOSE
					 {                                   //THE PLAYER WILL BE RE DIRECTED TO THE FOLLOWING...
						if (level==0||level==1)
							{			
								LCDclrAll();
								count=3;                       //EASY QUESTION
								delay_ms(500);
							}
								
						if(level==2)
							{
								LCDclrAll();
								count=12;                     //MEDIUM QUESTION
								delay_ms(500);
							}
						if (level>=3)
							{
								LCDclrAll();
								count=13;                    //HARDEST QUESTION
								delay_ms(500);
							}		
					 }
					while (UserButtonPress);           //ONLY EXECUTES WHEN PLAYER PRESSES BLUE BUTTON
				break;
					
			  case 3:                             //EASY QUESTION
					loadLine1();
					LCD_string ("The moon is made");
					loadLine2();
					LCD_string ("of cheese.");
					delay_ms(3000);
					count=4;                         //GIVES PLAYER CHANCE TO REVIEW THE QUESTION 
				break;                             //OR ANSWER STRAIGHT AWAY
				
				case 12:                          //MEDIUM QUESTION
					loadLine1();
					LCD_string ("How many bones");
					loadLine2();
					LCD_string ("do humans have");
					delay_ms(3000);
					count=4;                       //GIVES PLAYER CHANCE TO REVIEW THE QUESTION 
				break;	
					
				case 13:                         //HARD QUESTION
					LCD_string ("What is my");
					loadLine2();
					LCD_string ("favourite colour");
					delay_ms(3000);
				  count=4;                      //GIVES PLAYER CHANCE TO REVIEW THE QUESTION 
				break;
				
				 case 4:                        //REVIEW SCREEN
					 LCDclrAll();
					 loadLine1();
					 LCD_string("Blue 2 answer"); 
					 loadLine2();
					 LCD_string("A to replay");
					 delay_ms(1000);		
					 if (AButtonPress)            //IF USER PRESSES A...
						 {
							 LCDclrAll();
							 count=2;                //THEY WILL RETURN TO THE PREVIOUS QUESTION SCREEN
						 }
						 
					 
					 if (UserButtonPress)       //IF THE USER WANT TO ANSWER...
					 {
						if (level==0||level==1)   //...THEY WILL BE DIRECTED TO THE CORROSPONDING 
							{			                  //ANSWER SCREEN
								LCDclrAll();
								count=5;              //ANSWERS FOR EASY 
								delay_ms(500);
							}
								
						if(level==2)
							{
								LCDclrAll();
								count=14;             //ANSWERS FOR MEDIUM     
								delay_ms(500);
							}
									
						if (level>=3)
							{
								LCDclrAll();
								count=15;            //ANSWERS FOR HARD 
								delay_ms(500);
							}  	
					 } 				 
				break;

					
				case 5:
					LCDclrAll();
					loadLine1();
					LCD_string("A: True");
					loadLine2();
					LCD_string("B: False");
					delay_ms(1000);
						if (AButtonPress)         
						{	
							LCDclrAll();
							count=7;                //REDIRECT TO INCORRECT SCREEN
						}	
						
						if (BButtonPress)
						{
							LCDclrAll();
							count =6;              //REDIRECT TO CORRECT SCREEN
						}				
      	break;	
				
				case 14:
				LCDclrAll();
				loadLine1();
				LCD_string("A: 206");
				loadLine2();
				LCD_string("B: 257");
				delay_ms(1000);
					if (AButtonPress)
					{	
						LCDclrAll();
            count=6;             //REDIRECT TO CORRECT SCREEN
					}	
					
					if (BButtonPress)
					{
						LCDclrAll();
						count =7;           //REDIRECT TO INCORRECT SCREEN
					}
					
				break;
				
				
				case 15:
					LCDclrAll();
					loadLine1();
					LCD_string("A: GREEN");
					loadLine2();
					LCD_string("B: BURGANDY");
					delay_ms(1000);
					if (AButtonPress)
						{	
							LCDclrAll();
							count=6;                     //REDIRECT TO CORRECT SCREEN
						}	
						
						if (BButtonPress)
						{
							LCDclrAll();
							count =7;                    //REDIRECT TO INCORRECT SCREEN
						}	
				break;
				
				
				
				case 6:                            //IF USER ANSWERS CORRECT...
					  loadLine1();
						LCD_string("CORRECT!");
						victory_screech();             //...THEY GET A FAN FARE
				    delay_ms(5000);
				    score++;                       //1 POINT IS ADDED TO THEIR SCORE
				    LCDclrAll();
					  count = 8;                     //REDIRECT TO BUFFER SCREEN
      	break;
				
				case 7:                            //IF USER ANSWERS INCORRECT...
					loadLine1();
					LCD_string("INCORRECT!");
				  loadLine2();
				  sadchar();                       //...THEY GET A SAD FACE
					victory_screech2();              //AND A SAD FANFARE
					delay_ms(5000);
				  LCDclrAll();
					count = 8;                       //REDIRECT TO BUFFER SCREEN
				break;
				
				case 8:                            //MARKETING SCREEN FOR FUTURE OF GAME
					loadLine1();                     //SHOULD IT BE SUCCESSFUL
					LCD_string("FULL GAME...");
					loadLine2();
					LCD_string("COMING ONE DAY!?");
					delay_ms(5000);                  //GIVES USER TIME TO READ
					LCDclrAll();
					lcd_delayus(100);
					count = 9;                       //REDIRECT TO REPLAY SCREEN
				break;	
					
				case 9:
					loadLine1();          
					LCD_string(" PLAY AGAIN?");
					loadLine2();
				  LCD_string("A:YES B:NO");
					delay_ms(1000);
					if (AButtonPress)                //IF USER WANTS TO PLAY AGAIN...
						{
							LCDclrAll();
							count = 0;                  //...REDIRECT TO START OF GAME
						}
					if (BButtonPress)               //IF PLAYER WANT TO QUIT...
						{
							LCDclrAll();
							loadLine1();
					    LCD_string("HIGH SCORE!");  //HIGHSCORE IS DISPLAYED
							if(score==1)
							{
								loadLine2();
								LCD_string("1");
								HIGHSCORE();
								delay_ms(5000);
								LCDclrAll();
								count = 10;               //REDIRECT TO GAME OVER SCREEN
							}
							if(score==0)
							{
								loadLine2();
								LCD_string("0");
								HIGHSCORE();
								delay_ms(5000);
								LCDclrAll();
								count = 10;             //REDIRECT TO GAME OVER SCREEN
							}
						}
				break;
						
				case 10:
					loadLine1();
					LCD_string("GAME OVER");
				  delay_ms(5000);
				  LCDclrAll();
				  count =16;
				break;
				
				case 16:
					scroll2();                //THANKS FOR PLAYING ANIMATION
				break;
					

		
			}//END SWITCHES
	}//END WHILE
}//END MAIN
	

	
	