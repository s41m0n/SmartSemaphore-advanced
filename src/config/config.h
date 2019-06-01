#ifndef __CONFIG__
#define __CONFIG

/*	count = seconds * frequency */

#define PEDESTRIAN_TO_FLASHING 0x165A0BC0
#define PEDESTRIAN_TO_RED 0x7735940 		 
#define CAR_TO_YELLOW 0x7735940 				 
#define CAR_TO_RED 0x7735940						 
#define BLINKING_TIME 0xBEBC20	
#define SOUND_GREEN_TIME 0x17D7840
#define MANTEINANCE_TIME 0x17D7840

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */
/*k = 25Mhz / (450Hz * 45) = 1235*/
#define SOUND_FREQUENCE 1235

#define NORMAL_TIMER 0									 
#define	BLINKING_TIMER 1
#define SOUND_TIMER 2

enum leds{RedPed=7, GreenPed=6, RedCar=5, YellowCar=4, GreenCar=3}; 
enum states{GoPedestrian = 0, ChangingPed = 1, GoCar = 2, ChangingCar = 3, Manteinance = 4};

#endif
