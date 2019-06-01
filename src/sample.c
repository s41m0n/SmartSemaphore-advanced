/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------  */
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                  */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "config/config.h"
#include "adc/adc.h"

enum states currentState = GoPedestrian;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	ADC_init();														/* ADC Initialization, not enabled		*/
	
	LED_On(GreenPed);
	LED_On(RedCar);
	
	init_timer(SOUND_TIMER,1500);
	init_timer(NORMAL_TIMER, PEDESTRIAN_TO_FLASHING);	/* TIMER0 Initialization              */
	enable_timer(NORMAL_TIMER);
		
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	LPC_SC->PCON |= 0x1;				  					/* power-down	mode					*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
  while (1) {                           	/* Loop forever                       */	
		__ASM("wfi");
  }

}
