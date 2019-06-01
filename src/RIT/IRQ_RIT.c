/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../adc/adc.h"
#include "../led/led.h"
#include "../config/config.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

short down_0 = 0;
short down_1 = 0;
short down_2 = 0;
short requestForSound = 0;
extern short synchroOn;
extern enum states currentState;

void buttonPressed() {
	switch(currentState){
		/*Reset timer and stay in this state*/
		case GoPedestrian: {
			if(requestForSound){
				init_timer(BLINKING_TIMER, SOUND_GREEN_TIME);
				enable_timer(BLINKING_TIMER);
			}	
			reset_timer(NORMAL_TIMER);
			enable_timer(NORMAL_TIMER);
			break;
		};
		/*Go back into the normal green for pedestrian (GoPedestrian state)*/
		case ChangingPed: {
			disable_timer(BLINKING_TIMER);
			reset_timer(BLINKING_TIMER);
			currentState = GoPedestrian;
			if(synchroOn){
				synchroOn = !synchroOn;
				LED_On(GreenPed);
			}
			reset_timer(NORMAL_TIMER);
			init_timer(NORMAL_TIMER, PEDESTRIAN_TO_FLASHING);
			enable_timer(NORMAL_TIMER);
			if(requestForSound){
				init_timer(BLINKING_TIMER, SOUND_GREEN_TIME);
				enable_timer(BLINKING_TIMER);
			}	
			break;
		};
		/*Start the timer if it is not started yet*/
		case GoCar: {
			enable_timer(NORMAL_TIMER);
			break;
		};
		/*Perform a NOP as it is already changing state (going into GoPedestrian state)*/
		case ChangingCar:{
			__ASM("NOP");
			break;
		};
		default: break;
	}
	return;
}

void RIT_IRQHandler (void)
{					
	static short joystickCount = 0;
	
	/* button management INT0*/
	if(down_0!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_0++;				
			switch(down_0){
				case 2:
					buttonPressed();
					disable_timer(SOUND_TIMER);
					reset_timer(SOUND_TIMER);
					enable_timer(SOUND_TIMER);
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			disable_timer(SOUND_TIMER);
			reset_timer(SOUND_TIMER);
			requestForSound = 1;
			down_0 = 0;
			if(currentState == GoPedestrian || currentState == ChangingPed){
				disable_timer(BLINKING_TIMER);
				reset_timer(BLINKING_TIMER);
				init_timer(BLINKING_TIMER, SOUND_GREEN_TIME);
				enable_timer(BLINKING_TIMER);
			}
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	/* button management EINT1 */
	if(down_1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_1++;				
			switch(down_1){
				case 2:
					buttonPressed();
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	/* button management EINT2 */
	if(down_2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_2++;				
			switch(down_2){
				case 2:
					buttonPressed();
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
		
	/* joystick management */
	if(currentState == GoPedestrian && !(LPC_GPIO1->FIOPIN & (1<<28))){	/* Joytick RIGHT pressed */
		joystickCount++;
		switch(joystickCount){
			case 1:
				currentState = Manteinance;
				disable_timer(NORMAL_TIMER);
				reset_timer(NORMAL_TIMER);
				init_timer(BLINKING_TIMER, MANTEINANCE_TIME);
				enable_timer(BLINKING_TIMER);
				LED_Off(GreenPed);
				LED_Off(RedCar);
				ADC_enable();
				break;
			default:
				break;
		}
	} else if(currentState == Manteinance && !(LPC_GPIO1->FIOPIN & (1<<27))) {
			joystickCount++;
			switch(joystickCount){
				case 1:
					currentState= GoPedestrian;
					LED_Off(YellowCar);
					LED_Off(RedPed);
					LED_On(GreenPed);
					LED_On(RedCar);
					enable_timer(NORMAL_TIMER);
					disable_timer(BLINKING_TIMER);
					reset_timer(BLINKING_TIMER);
					if(requestForSound){
						init_timer(BLINKING_TIMER, SOUND_GREEN_TIME);
						enable_timer(BLINKING_TIMER);
					}
					ADC_disable();
					break;
				default:
					break;
			}
	} else
		joystickCount=0;
		
	/* ADC management */
	if(currentState == Manteinance)
		ADC_start_conversion();		
			
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
