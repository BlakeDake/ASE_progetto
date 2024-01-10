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

typedef enum Turn {
	Player1,
	Player2
} Turn;

Direction token_dir;
uint8_t board[BOARD_LENGTH][BOARD_LENGTH] = {0};
Pos_square player1 = {0,3};
Pos_square player2 = {6,3};
Turn turn = Player1;

void paint_left_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(board[2*position.row][2*position.column-1] == 0) {					// check barrier
		if(board[2*position.row][2*position.column-2] == 0) {				// check jump over opponent
			paint_square(position.row, position.column-1, color);			// show if no barrier and no opponent
			token_dir = Left;
		} else {																										// opponent is there
			if(position.column != 1) {																// the opponent is on the border, so you are in the second column
				if(board[2*position.row][2*position.column-3] == 0) {		// check barrier behind opponent
					paint_square(position.row, position.column-2, color);
					token_dir = Left;
				}
			}
		}
	}
}

void paint_right_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(board[2*position.row][2*position.column+1] == 0) {
		if(board[2*position.row][2*position.column+2] == 0) {
			paint_square(position.row, position.column+1, color);
			token_dir = Right;
		} else {
			if(position.column != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row][2*position.column+3] == 0) {
					paint_square(position.row, position.column+2, color);
					token_dir = Right;
				}
			}
		}
	}
}

void paint_up_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(board[2*position.row-1][2*position.column] == 0) {
		if(board[2*position.row-2][2*position.column] == 0) {
			paint_square(position.row-1, position.column, color);
			token_dir = Up;
		} else {
			if(position.row != 1) {
				if(board[2*position.row-3][2*position.column] == 0) {
					paint_square(position.row-2, position.column, color);
					token_dir = Up;
				}
			}
		}
	}
}

void paint_down_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(board[2*position.row+1][2*position.column] == 0) {
		if(board[2*position.row+2][2*position.column] == 0) {
			paint_square(position.row+1, position.column, color);
			token_dir = Down;
		} else {
			if(position.row != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row+3][2*position.column] == 0) {
					paint_square(position.row+2, position.column, color);
					token_dir = Down;
				}
			}
		}
	}
}

void Show_Possible_Moves(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(position.row == 0 && position.column == 0) {		// top left corner
		paint_right_square(board, position, color);
		paint_down_square(board, position, color);
	} else if(position.row == 0 && position.column== NUM_SQUARE_PER_SIDE-1) {		// top right corner
		paint_left_square(board, position, color);
		paint_down_square(board, position, color);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1 && position.column == 0) {		// bottom left corner
		paint_up_square(board, position, color);
		paint_right_square(board, position, color);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1 && position.column == NUM_SQUARE_PER_SIDE-1) {		// bottom right
		paint_left_square(board, position, color);
		paint_up_square(board, position, color);
	} else if(position.column == 0) {		// left border 
		paint_up_square(board, position, color);
		paint_down_square(board, position, color);
		paint_right_square(board, position, color);
	} else if(position.column == NUM_SQUARE_PER_SIDE-1) {	// right border
		paint_left_square(board, position, color);
		paint_up_square(board, position, color);
		paint_down_square(board, position, color);
	} else if(position.row == 0) {	// top border
		paint_left_square(board, position, color);
		paint_right_square(board, position, color);
		paint_down_square(board, position, color);
	} else if(position.row == NUM_SQUARE_PER_SIDE-1) {	// bottom border
		paint_up_square(board, position, color);
		paint_left_square(board, position, color);
		paint_right_square(board, position, color);
	} else if((position.row != 0 && position.column != 0) || 
						(position.row != NUM_SQUARE_PER_SIDE-1 && position.column != NUM_SQUARE_PER_SIDE-1)) {	// not on the border
		paint_left_square(board, position, color);
		paint_right_square(board, position, color);
		paint_up_square(board, position, color);
		paint_down_square(board, position, color);
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

void show_decision_square(Direction dir) {
	switch(dir) {
		case Up:
			switch(turn) {
				case Player1:
					Show_Possible_Moves(board, player1, Yellow);
					paint_up_square(board, player1, Green);
				break;
				case Player2:
					Show_Possible_Moves(board, player2, Yellow);
					paint_up_square(board, player2, Green);
				break;
			}
			break;
		case Down:
			switch(turn) {
				case Player1:
					Show_Possible_Moves(board, player1, Yellow);
					paint_down_square(board, player1, Green);
				break;
				case Player2:
					Show_Possible_Moves(board, player2, Yellow);
				paint_down_square(board, player2, Green);
				break;
			}
			break;
		case Left:
			switch(turn) {
				case Player1:
					Show_Possible_Moves(board, player1, Yellow);
					paint_left_square(board, player1, Green);
				break;
				case Player2:
					Show_Possible_Moves(board, player2, Yellow);
					paint_left_square(board, player2, Green);
				break;
			}
			break;
		case Right:
			switch(turn) {
				case Player1:
					Show_Possible_Moves(board, player1, Yellow);
					paint_right_square(board, player1, Green);
				break;
				case Player2:
					Show_Possible_Moves(board, player2, Yellow);
					paint_right_square(board, player2, Green);
				break;
			}
			break;
		case Select:
			switch(turn) {
				case Player1:
					turn = Player2;
					break;
				case Player2:
					turn = Player1;
					break;
			}
			break;
	}
}

void Start_Game(void) {
	Move init_p1 = {0,3,0,3};
	Move init_p2 = {6,3,6,3};
	Pos_barrier bar1 = {1,2,1};

	init_timer(0, 0x00B71B00);
	enable_timer(0);

	update_board_player_move(board, init_p1);
	update_board_player_move(board, init_p2);
	update_board_barrier(board, bar1);

	Show_Possible_Moves(board, player1, Yellow);
}
