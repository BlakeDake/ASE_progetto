#include "button.h"
#include "lpc17xx.h"

#include "game_logic.h"
#include "timer/timer.h"

Button_Pressed int0 = Not_Pressed;

#if COMPILE_DEBOUNCING

extern int down_0;
extern int down_1;
extern int down_2;

void EINT0_IRQHandler(void) {			// start game
  //LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	//if(int0 == Not_Pressed) {
	//	timer_value = 20;
	//	int0 = Pressed;
	//	Start_Game();
	//}
	down_0 = 1;
	NVIC_DisableIRQ(EINT0_IRQn);					// disable Button interrupts
	LPC_PINCON->PINSEL4 &= ~(1 << 20);		// GPIO pin selection
	LPC_SC->EXTINT &= (1 << 0);					 	// clear pending interrupt
}

void EINT1_IRQHandler(void)	{			// switch token/wall mode
	//LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	//if(int0 == Pressed) {
	//	routine_mode();
	//}
	down_1 = 1;
	NVIC_DisableIRQ(EINT1_IRQn);					// disable Button interrupts
	LPC_PINCON->PINSEL4 &= ~(1 << 22);		// GPIO pin selection
	LPC_SC->EXTINT &= (1 << 1);						// clear pending interrupt
}

void EINT2_IRQHandler (void) {		// rotate wall
  //LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
	
	//switch(mode) {
	//case Token:
	//		break;
	//case Wall:
	//		if(int0 == Pressed) {
	//		show_wall_movement(Wall_Rotate);
	//		}
	//		break;
	//}
	down_2 = 1;
	NVIC_DisableIRQ(EINT2_IRQn);					// disable Button interrupts
	LPC_PINCON->PINSEL4 &= ~(1 << 24);		// GPIO pin selection
	LPC_SC->EXTINT &= (1 << 2);							// clear pending interrupt
}

#else 

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

#endif
