#include "coloring.h"

void paint_square(uint8_t x, uint8_t y, uint16_t color) {
	uint16_t pixel_x_start = 0;
	uint16_t pixel_y_start = 0;
	uint16_t pixel_x_end = 0;
	int i;
	
	if(x > NUM_SQUARE_PER_SIDE-1 || y > NUM_SQUARE_PER_SIDE-1) {
		return;
	}
	pixel_x_start = GRID_BORDER_GAP + (SQUARE_SIDE_LENGTH*x) + (SQUARE_GAP*x) + 1;
	pixel_y_start = GRID_BORDER_GAP + (SQUARE_SIDE_LENGTH*y) + (SQUARE_GAP*y) + 1;
	pixel_x_end = pixel_x_start + SQUARE_SIDE_LENGTH - 2;
	
	for(i = 0; i < SQUARE_SIDE_LENGTH-1; i++) {
		LCD_DrawLine(pixel_x_start, pixel_y_start+i, pixel_x_end, pixel_y_start+i, color);
	}
}
