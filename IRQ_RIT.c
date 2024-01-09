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
#include "game_logic.h"

volatile int mossa = 0;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{
	static int js_up=0;
	static int js_down=0;
	static int js_right=0;
	static int js_left=0;
	static int select=0;
	static int down=0;
	static uint8_t position=0;
	down++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
		//reset_RIT();
		switch(down){
			case 1:
				if( position == 7){
					//LED_On(0);
					//LED_Off(7);
					position = 0;
				}
				else{
					//LED_Off(position);
					//LED_On(++position);
				}
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down=0;			
		//disable_RIT();
		//reset_RIT();
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	 // 26 down 27 left 28 right 29 up
		/* Joytick UP pressed */
		js_up++;
		switch(js_up){
			case 1:
				// Spostamento in alto
				break;
			
			default:
				break;
		}
	}
	else{
			js_up=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		/* Joytick right pressed */
		js_right++;
		switch(js_right){
			case 1:
				// Spostamento a destra
				break;
			
			default:
				break;
		}
	}
	else{
			js_right=0;
	}
	
			if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		/* Joytick left pressed */
		js_left++;
		switch(js_left){
			case 1:
				// Spostamento a sinistra
				break;

			default:
				break;
		}
	}
	else{
			js_left=0;
	}
	
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		/* Joytick down pressed */
		js_down++;
		switch(js_down){
			case 1:
				// Spostamento in basso
				break;

			default:
				break;
		}
	}
	else{
			js_down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){								//il joystick (il voltaggio è basso [==0] quando il tasto è premuto
		/* Joytick select pressed */
		select++;
		switch(select){										//lo switch su up per fare un polling di un tot di tempo
			case 1:
				// conferma mossa
				break;

			default:
				break;
		}
	}else{
			select = 0;
	}

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
