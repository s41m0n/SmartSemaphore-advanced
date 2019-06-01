#include "button.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../config/config.h"

extern short down_0;
extern short down_1;
extern short down_2;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	down_0=1;
			
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}

void EINT1_IRQHandler (void)	  	/* KEY1, bit 11											 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down_1=1;
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2, bit 12											 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	down_2=1;
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}
