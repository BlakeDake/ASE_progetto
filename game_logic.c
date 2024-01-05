#include "game_logic.h"

#define BOARD_LENGTH 13		// 7 squares + 6 gap for walls

typedef struct position {
	uint8_t x;
	uint8_t y;
} Pos;

void paint_left_square(Pos position) {
	paint_square(position.x-1, position.y, Yellow);
}

void paint_right_square(Pos position) {
	paint_square(position.x+1, position.y, Yellow);
}

void paint_up_square(Pos position) {
	paint_square(position.x, position.y+1, Yellow);
}

void paint_down_square(Pos position) {
	paint_square(position.x, position.y-1, Yellow);
}

void Show_Possible_Moves(uint8_t board[][BOARD_LENGTH], Pos position) {
	if((position.x != 0 && position.y != 0) || 
		 (position.x != NUM_SQUARE_PER_SIDE-1 && position.y != NUM_SQUARE_PER_SIDE-1)) {	// not on the border
		paint_left_square(position);
		paint_right_square(position);
		paint_up_square(position);
		paint_down_square(position);
	} else if(position.x == 0 && position.y == 0) {		// top left corner
		paint_right_square(position);
		paint_down_square(position);
	} else if(position.x == NUM_SQUARE_PER_SIDE-1 && position.y == 0) {		// top right corner
		paint_left_square(position);
		paint_down_square(position);
	} else if(position.x == 0 && position.y == NUM_SQUARE_PER_SIDE-1) {		// bottom left corner
		paint_up_square(position);
		paint_right_square(position);
	} else if(position.x == NUM_SQUARE_PER_SIDE-1 && position.y == NUM_SQUARE_PER_SIDE-1) {		// bottom right
		paint_left_square(position);
		paint_up_square(position);
	} else if(position.x == 0) {		// top border 
		paint_left_square(position);
		paint_down_square(position);
		paint_right_square(position);
	} else if(position.x == NUM_SQUARE_PER_SIDE-1) {	// bottom border
		paint_left_square(position);
		paint_up_square(position);
		paint_right_square(position);
	} else if(position.y == 0) {	// left border
		paint_up_square(position);
		paint_right_square(position);
		paint_down_square(position);
	} else if(position.y == NUM_SQUARE_PER_SIDE-1) {	// right border
		paint_up_square(position);
		paint_left_square(position);
		paint_down_square(position);
	}
}


void Start_Game(void) {
	uint8_t player_turn = 1;
	uint8_t board[BOARD_LENGTH][BOARD_LENGTH] = {0};
	Pos player1 = {3,0};
	Pos player2 = {3,6};
	
	init_timer(0, 0x00B71B00);
	enable_timer(0);
	
	Show_Possible_Moves(board, player1);	
}
