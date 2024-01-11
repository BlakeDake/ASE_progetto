#include "button.h"
#include "lpc17xx.h"

#include "game_logic.h"
#include "timer/timer.h"

Button_Pressed int0 = Not_Pressed;

void EINT0_IRQHandler(void) {			// start game
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	if(int0 == Not_Pressed) {
		timer_value = 20;
		int0 = Pressed;
		Start_Game();
	}
}

void EINT1_IRQHandler(void)	{			// switch token/wall mode
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	if(int0 == Pressed) {
		routine_mode();
	}
}

void EINT2_IRQHandler (void) {		// rotate wall
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
	
	switch(mode) {
		case Token:
			break;
		case Wall:
			if(int0 == Pressed) {
				show_wall_movement(Wall_Rotate);
			}
			break;
	}
}
