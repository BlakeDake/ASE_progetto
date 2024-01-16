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
#include "button.h"

#if COMPILE_DEBOUNCING

volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler(void) {
	static int js_up = 0;
	static int js_down = 0;
	static int js_right = 0;
	static int js_left = 0;
	static int js_select = 0;

	if(down_0 != 0) {
		down_0++;
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0) {
			switch(down_0) {
				case 2:
					if(int0 == Not_Pressed) {
						timer_value = 20;
						int0 = Pressed;
						Start_Game();
					}
					break;
				default:
					break;
			}
		} else {
			down_0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1<<20);
		}
	}
	
	if(int0 == Pressed) {			// no button should do anything until int0 has been pressed
		if(down_1 != 0) {
		down_1++;
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0) {
			switch(down_1) {
				case 2:
					if(int0 == Pressed) {
						routine_mode();
					}
					break;
				default:
					break;
			}
		} else {
			down_1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);
			LPC_PINCON->PINSEL4 |= (1<<22);
		}
	}
	
	if(down_2 != 0) {
		down_2++;
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0) {
			switch(down_2) {
				case 2:
					switch(mode) {
						case Token:
							break;
						case Wall:
							if(int0 == Pressed)
								show_wall_movement(Wall_Rotate);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			down_2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);
			LPC_PINCON->PINSEL4 |= (1<<24);
		}
	}
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {		// 29 = UP
			js_up++;
			switch(js_up) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Up);
							break;
						case Wall:
							show_wall_movement(Wall_Up);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			js_up = 0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {		// 28 = RIGHT
			js_right++;
			switch(js_right) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Right);
							break;
						case Wall:
							show_wall_movement(Wall_Right);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			js_right=0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {		// 27 = LEFT
			js_left++;
			switch(js_left){
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Left);
							break;
						case Wall:
							show_wall_movement(Wall_Left);
							break;
					}
					break;
				default:
					break;
			}
		}	else {
			js_left=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {			// 26 = DOWN
			js_down++;
			switch(js_down) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Down);
							break;
						case Wall:
							show_wall_movement(Wall_Down);
							break;
					}
				default:
					break;
			}
		}	else {
			js_down=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {		// 25 = SELECT (joystick pressed)
			js_select++;
			switch(js_select) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Select);
							if(victory) {
								disable_timer(0);
								switch(turn) {
									case Player1:
										GUI_Text(SIDE_RECTANGLE_GAP+2,
														 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
														 "Player1 wins!",
														 Black,
														 White);
										break;
									case Player2:
										GUI_Text(SIDE_RECTANGLE_GAP+2,
														 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
														 "Player2 wins!",
														 Black,
														 White);
										break;
								}
							} else if(swap) {
								swap = 0;
								new_turn(Player);
							}
							break;
						case Wall:
							if(show_wall_movement(Wall_Select)) {		// if the wall is placeable, then swap turn
								new_turn(Barrier);
							}
							break;
					}
				default:
					break;
			}
		} else {
			js_select = 0;
		}
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */

  return;
}

#else

void RIT_IRQHandler(void) {
	static int js_up = 0;
	static int js_down = 0;
	static int js_right = 0;
	static int js_left = 0;
	static int select = 0;

	if(int0 == Pressed) {
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {		// 29 = UP
			js_up++;
			switch(js_up) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Up);
							break;
						case Wall:
							show_wall_movement(Wall_Up);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			js_up = 0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {		// 28 = RIGHT
			js_right++;
			switch(js_right) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Right);
							break;
						case Wall:
							show_wall_movement(Wall_Right);
							break;
					}
					break;
				default:
					break;
			}
		} else {
			js_right=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {		// 27 = LEFT
			js_left++;
			switch(js_left){
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Left);
							break;
						case Wall:
							show_wall_movement(Wall_Left);
							break;
					}
					break;
				default:
					break;
			}
		}	else {
			js_left=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {			// 26 = DOWN
			js_down++;
			switch(js_down) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Down);
							break;
						case Wall:
							show_wall_movement(Wall_Down);
							break;
					}
				default:
					break;
			}
		}	else {
			js_down=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {		// 25 = SELECT (joystick pressed)
			select++;
			switch(select) {
				case 1:
					switch(mode) {
						case Token:
							show_decision_square(Select);
							if(victory) {
								disable_timer(0);
								switch(turn) {
									case Player1:
										GUI_Text(SIDE_RECTANGLE_GAP+2,
														 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
														 "Player1 wins!",
														 Black,
														 White);
										break;
									case Player2:
										GUI_Text(SIDE_RECTANGLE_GAP+2,
														 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
														 "Player2 wins!",
														 Black,
														 White);
										break;
								}
							} else if(swap) {
								swap = 0;
								new_turn(Player);
							}
							break;
						case Wall:
							if(show_wall_movement(Wall_Select)) {		// if the wall is placeable, then swap turn
								new_turn(Barrier);
							}
							break;
					}
				default:
					break;
			}
		} else {
			select = 0;
		}
	}
  return;
}

#endif


/******************************************************************************
**                            End Of File
******************************************************************************/
