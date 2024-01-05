/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"

#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "../init.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint8_t timer_value;

void TIMER0_IRQHandler (void)
{
	char buffer[3];
	if (timer_value > 0) {
		timer_value -= 1;
		sprintf(buffer, "%d", timer_value);
		GUI_Text(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+3,
					   GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2,
						 "  ",
						 Black,
						 White);			// first I clear the screen
		GUI_Text(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+3,
					   GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2,
						 (uint8_t *)buffer,
						 Black,
						 White);			// then I print the timer
		reset_timer(0);
	} else {
		disable_timer(0);
	}
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
