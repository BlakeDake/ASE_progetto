#include "coloring.h"

void paint_square(uint8_t row, uint8_t column, uint16_t color) {
	uint16_t pixel_x_start = 0;
	uint16_t pixel_y_start = 0;
	uint16_t pixel_x_end = 0;
	int i;
	
	if(row > NUM_SQUARE_PER_SIDE-1 || column > NUM_SQUARE_PER_SIDE-1) {
		return;
	}
	pixel_x_start = GRID_BORDER_GAP + (SQUARE_SIDE_LENGTH*column) + (SQUARE_GAP*column) + 1;
	pixel_y_start = GRID_BORDER_GAP + (SQUARE_SIDE_LENGTH*row) + (SQUARE_GAP*row) + 1;
	pixel_x_end = pixel_x_start + SQUARE_SIDE_LENGTH - 2;
	
	for(i = 0; i < SQUARE_SIDE_LENGTH-1; i++) {
		LCD_DrawLine(pixel_x_start, pixel_y_start+i, pixel_x_end, pixel_y_start+i, color);
	}
}
