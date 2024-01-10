#include "button.h"
#include "lpc17xx.h"

#include "game_logic.h"
#include "timer/timer.h"

enum INT0 int0 = Not_Pressed;

void EINT0_IRQHandler (void)
{
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	timer_value = 20;
	int0 = Pressed;
	Start_Game();
}


void EINT1_IRQHandler (void)	  
{
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


