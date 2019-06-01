/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../config/config.h"
#include "../led/led.h"
#include "../adc/adc.h"

extern unsigned short AD_current;
extern enum states currentState;
extern short requestForSound;
short synchroOn = 1;

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    205, 233, 261, 288, 313, 336, 357, 374, 389, 399, 406, 409, 408, 403, 394, 382,
		366, 347, 325, 301, 275, 247, 219, 190, 162, 135, 108, 84, 62, 43, 27, 15, 6, 1,
		0, 3, 10, 20, 35, 52, 73, 96, 121, 148, 176
};

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{	 
	
	switch(currentState){
		case GoPedestrian: {
			currentState = ChangingPed;
			init_timer(NORMAL_TIMER, PEDESTRIAN_TO_RED);
			disable_timer(BLINKING_TIMER);
			reset_timer(BLINKING_TIMER);
			init_timer(BLINKING_TIMER, BLINKING_TIME);
			enable_timer(BLINKING_TIMER);
			enable_timer(NORMAL_TIMER);
			synchroOn = 0;
			break;
		};
		case ChangingPed: {
			disable_timer(BLINKING_TIMER);
			reset_timer(BLINKING_TIMER);
			disable_timer(SOUND_TIMER);
			reset_timer(SOUND_TIMER);
			disable_timer(NORMAL_TIMER);
			requestForSound = 0;
			synchroOn = 0;
			currentState = GoCar;
			init_timer(NORMAL_TIMER, CAR_TO_YELLOW);
			LED_Off(GreenPed);
			LED_Off(RedCar);
			LED_On(RedPed);
			LED_On(GreenCar);
			break;
		};
		case GoCar: {
			currentState = ChangingCar;
			init_timer(NORMAL_TIMER, CAR_TO_RED);
			enable_timer(NORMAL_TIMER);
			LED_Off(GreenCar);
			LED_On(YellowCar);
			break;
		};
		case ChangingCar:{
			if(requestForSound){
				init_timer(BLINKING_TIMER, SOUND_GREEN_TIME);
				enable_timer(BLINKING_TIMER);
				enable_timer(SOUND_TIMER);
			}	
			currentState = GoPedestrian;
			init_timer(NORMAL_TIMER, PEDESTRIAN_TO_FLASHING);
			enable_timer(NORMAL_TIMER);
			LED_Off(YellowCar);
			LED_Off(RedPed);
			LED_On(RedCar);
			LED_On(GreenPed);
			break;
		};
		default: break;
	}	
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
	return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(synchroOn){
		if(requestForSound || currentState == Manteinance)
			enable_timer(SOUND_TIMER);
		if(currentState == Manteinance){
			LED_On(RedPed);
			LED_On(YellowCar);
		} else if(currentState == ChangingPed){
			LED_On(GreenPed);
		}
	}else{
		if(requestForSound || currentState == Manteinance){
			disable_timer(SOUND_TIMER);
			reset_timer(SOUND_TIMER);
		}
		if(currentState == Manteinance){
			LED_Off(RedPed);
			LED_Off(YellowCar);
		} else if(currentState == ChangingPed){
			LED_Off(GreenPed);
		}
	}
	synchroOn = !synchroOn;
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

void TIMER2_IRQHandler (void){
	static short ticks=0;
	LPC_DAC->DACR = (SinTable[ticks] * (AD_current*10/0xFFF))<<6;
	ticks++;
	if(ticks==45) ticks=0;
	
	LPC_TIM2->IR = 1;
	return;
}
