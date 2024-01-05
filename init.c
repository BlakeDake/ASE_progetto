#include "init.h"

void Draw_Square(uint16_t x, uint16_t y) {
	LCD_DrawLine(x, y, x, y+SQUARE_SIDE_LENGTH, Black);																					// left vertical side
	LCD_DrawLine(x, y, x+SQUARE_SIDE_LENGTH, y, Black);																					// upper horizontal side
	LCD_DrawLine(x+SQUARE_SIDE_LENGTH, y, x+SQUARE_SIDE_LENGTH, y+SQUARE_SIDE_LENGTH, Black);		// right vertical side
	LCD_DrawLine(x, y+SQUARE_SIDE_LENGTH, x+SQUARE_SIDE_LENGTH, y+SQUARE_SIDE_LENGTH, Black);		// lower horizontal side
	
	return;
}

void Draw_Grid() {
	int i, j;
	int x = GRID_BORDER_GAP;
	int y = GRID_BORDER_GAP;
	for(i = 0; i < NUM_SQUARE_PER_SIDE; i++) {
		for(j = 0; j < NUM_SQUARE_PER_SIDE; j++) {
			Draw_Square(x, y);
			x += SQUARE_SIDE_LENGTH + SQUARE_GAP;
		}
		y += SQUARE_SIDE_LENGTH + SQUARE_GAP;
		x = GRID_BORDER_GAP;
	}
	
	return;
}

void Fill_Square(uint16_t x, uint16_t y, uint16_t color) {
	int i;
	for(i = 0; i < SQUARE_SIDE_LENGTH-1; i++) {
			LCD_DrawLine(x, y+i, x+SQUARE_SIDE_LENGTH-2, y+i, color);
	}
	
	return;
}

void Draw_Rectangle(uint16_t x, uint16_t y, uint16_t horizontal_length, uint16_t vertical_length) {
	LCD_DrawLine(x, y, x, y+vertical_length, Black);																						// left vertical side
	LCD_DrawLine(x, y, x+horizontal_length, y, Black);																					// upper horizontal side
	LCD_DrawLine(x+horizontal_length, y, x+horizontal_length, y+vertical_length, Black);				// right vertical side
	LCD_DrawLine(x, y+vertical_length, x+horizontal_length, y+vertical_length, Black);					// lower horizontal side
	
	return;
}

void Draw_Starting_Token(uint16_t player_num) {
	switch(player_num) {
		case 1:
			Fill_Square(GRID_BORDER_GAP+(3*SQUARE_SIDE_LENGTH)+(3*SQUARE_GAP)+1, 
								  GRID_BORDER_GAP+1, 
									Blue);
			break;
		case 2:
			Fill_Square(GRID_BORDER_GAP+(3*SQUARE_SIDE_LENGTH)+(3*SQUARE_GAP)+1, 
									GRID_BORDER_GAP+(6*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+1,
									Red);
			break;
		default:
			break;
	}
	
	return;
}

void Draw_Start_Screen() {
	Draw_Grid();
	
	Draw_Rectangle(SIDE_RECTANGLE_GAP, 
								 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP, 
								 RECTANGLE_HORIZONTAL_SIDE_LENGTH, 
								 RECTANGLE_VERTICAL_SIDE_LENGTH);
	Draw_Rectangle(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP, 
								 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP, 
								 RECTANGLE_HORIZONTAL_SIDE_LENGTH, 
								 RECTANGLE_VERTICAL_SIDE_LENGTH);
	Draw_Rectangle(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP, 
								 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP, 
								 RECTANGLE_HORIZONTAL_SIDE_LENGTH, 
								 RECTANGLE_VERTICAL_SIDE_LENGTH);
	GUI_Text(SIDE_RECTANGLE_GAP+2,
					 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2,
					 "P1 Wall",
					 Black,
					 White);
	GUI_Text(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+2,
					 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2,
					 "P2 Wall",
					 Black,
					 White);
	GUI_Text(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+3,
					   GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2,
						 "20",
						 Black,
						 White);
	Draw_Starting_Token(1);
	Draw_Starting_Token(2);
	
	return;
}
