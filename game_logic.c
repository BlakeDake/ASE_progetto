#include "game_logic.h"

#define BOARD_LENGTH 13		// 7 squares + 6 gap for walls

typedef struct position {
	uint8_t row;
	uint8_t column;
} Pos_square;

typedef struct position_barrier {
	uint8_t row;
	uint8_t column;
	uint8_t direction;		// 0 = horizontal, 1 = vertical
} Pos_barrier;

typedef struct move {
	uint8_t old_row;
	uint8_t old_column;
	uint8_t new_row;
	uint8_t new_column;
} Move;

typedef struct possible_token_dir {
	uint8_t up;
	uint8_t down;
	uint8_t left;
	uint8_t right;
} Possible_Token_Dir;

void paint_left_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color, Possible_Token_Dir* token_dir) {
	if(board[2*position.row][2*position.column-1] == 0) {					// check barrier
		if(board[2*position.row][2*position.column-2] == 0) {				// check jump over opponent
			paint_square(position.row, position.column-1, color);			// show if no barrier and no opponent
			token_dir->left = 1;
		} else {																										// opponent is there
			if(position.column != 1) {																// the opponent is on the border, so you are in the second column
				if(board[2*position.row][2*position.column-3] == 0) {		// check barrier behind opponent
					paint_square(position.row, position.column-2, color);
					token_dir->left = 1;
				}
			}
		}
	}
}

void paint_right_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color, Possible_Token_Dir* token_dir) {
	if(board[2*position.row][2*position.column+1] == 0) {
		if(board[2*position.row][2*position.column+2] == 0) {
			paint_square(position.row, position.column+1, color);
			token_dir->right = 1;
		} else {
			if(position.column != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row][2*position.column+3] == 0) {
					paint_square(position.row, position.column+2, color);
					token_dir->right = 1;
				}
			}
		}
	}
}

void paint_up_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color, Possible_Token_Dir* token_dir) {
	if(board[2*position.row-1][2*position.column] == 0) {
		if(board[2*position.row-2][2*position.column] == 0) {
			paint_square(position.row-1, position.column, color);
			token_dir->up = 1;
		} else {
			if(position.row != 1) {
				if(board[2*position.row-3][2*position.column] == 0) {
					paint_square(position.row-2, position.column, color);
					token_dir->up = 1;
				}
			}
		}
	}
}

void paint_down_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color, Possible_Token_Dir* token_dir) {
	if(board[2*position.row+1][2*position.column] == 0) {
		if(board[2*position.row+2][2*position.column] == 0) {
			paint_square(position.row+1, position.column, color);
			token_dir->down = 1;
		} else {
			if(position.row != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row+3][2*position.column] == 0) {
					paint_square(position.row+2, position.column, color);
					token_dir->down = 1;
				}
			}
		}
	}
}

void Show_Possible_Moves(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color, Possible_Token_Dir* token_dir) {
	if(position.row == 0 && position.column == 0) {		// top left corner
		paint_right_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
	} else if(position.row == 0 && position.column== NUM_SQUARE_PER_SIDE-1) {		// top right corner
		paint_left_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1 && position.column == 0) {		// bottom left corner
		paint_up_square(board, position, color, token_dir);
		paint_right_square(board, position, color, token_dir);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1 && position.column == NUM_SQUARE_PER_SIDE-1) {		// bottom right
		paint_left_square(board, position, color, token_dir);
		paint_up_square(board, position, color, token_dir);
	} else if(position.column == 0) {		// left border 
		paint_up_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
		paint_right_square(board, position, color, token_dir);
	} else if(position.column == NUM_SQUARE_PER_SIDE-1) {	// right border
		paint_left_square(board, position, color, token_dir);
		paint_up_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
	} else if(position.row == 0) {	// top border
		paint_left_square(board, position, color, token_dir);
		paint_right_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1) {	// bottom border
		paint_up_square(board, position, color, token_dir);
		paint_left_square(board, position, color, token_dir);
		paint_right_square(board, position, color, token_dir);
	} else if((position.row != 0 && position.column != 0) || 
						(position.row != NUM_SQUARE_PER_SIDE-1 && position.column != NUM_SQUARE_PER_SIDE-1)) {	// not on the border
		paint_left_square(board, position, color, token_dir);
		paint_right_square(board, position, color, token_dir);
		paint_up_square(board, position, color, token_dir);
		paint_down_square(board, position, color, token_dir);
	}
}

void update_board_player_move(uint8_t board[][BOARD_LENGTH], Move move) {
	board[2*move.old_row][2*move.old_column] = 0;
	board[2*move.new_row][2*move.new_column] = 1;
}

void update_board_barrier(uint8_t board[][BOARD_LENGTH], Pos_barrier barrier) {
	switch(barrier.direction) {
		case 0:			// horizontal
			board[2*barrier.row+1][2*barrier.column] = 1;
			board[2*barrier.row+1][2*barrier.column+2] = 1;
			paint_barrier(barrier.row, barrier.column, barrier.direction, Magenta);
			break;
		case 1:			// vertical
			board[2*barrier.row][2*barrier.column+1] = 1;
			board[2*barrier.row+2][2*barrier.column+1] = 1;
			paint_barrier(barrier.row, barrier.column, barrier.direction, Magenta);
			break;
		default:
			break;
	}
}


void Start_Game(void) {
	uint8_t board[BOARD_LENGTH][BOARD_LENGTH] = {0};
	Pos_square player1 = {0,3};
	Pos_square player2 = {6,3};
	Move init_p1 = {0,3,0,3};
	Move init_p2 = {6,3,6,3};
	Pos_barrier bar1 = {1,2,1};
	Possible_Token_Dir token_dir = {0,0,0,0};

	init_timer(0, 0x00B71B00);
	enable_timer(0);

	update_board_player_move(board, init_p1);
	update_board_player_move(board, init_p2);
	update_board_barrier(board, bar1);

	Show_Possible_Moves(board, player1, Yellow, &token_dir);
	Show_Possible_Moves(board, player1, White, &token_dir);
}
