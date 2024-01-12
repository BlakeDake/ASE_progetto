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
	static int select = 0;
	static int down = 0;
	static uint8_t position = 0;
	
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0) {			// debouncing of buttons TODO
		down++;
		//reset_RIT();
		switch(down){
			case 1:
				if( position == 7){
					position = 0;
				}
				else{

				}
				break;
			default:
				break;
		}
	} else {	/* button released */
		down=0;			
		//disable_RIT();
		//reset_RIT();
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
	}
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
								new_turn();
							}
							break;
						case Wall:
							show_wall_movement(Wall_Select);
							new_turn();
							break;
					}
				default:
					break;
			}
		} else {
			select = 0;
		}
	}
  //LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */

  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
