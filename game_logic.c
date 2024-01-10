#include "game_logic.h"

#define BOARD_LENGTH 13		// 7 squares + 6 gap for walls

typedef struct position {
	uint8_t row;
	uint8_t column;
} Pos_square;

typedef struct position_barrier {
	uint8_t row;
	uint8_t column;
	uint8_t direction;			// 0 = horizontal, 1 = vertical
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

uint8_t swap = 0;
uint8_t victory = 0;
Direction token_dir = Nothing;
uint8_t board[BOARD_LENGTH][BOARD_LENGTH] = {0};
Pos_square player1 = {0,3};
Pos_square player2 = {6,3};
Turn turn = Player1;

void paint_left_square(uint8_t board[][BOARD_LENGTH], Pos_square position, uint16_t color) {
	if(board[2*position.row][2*position.column-1] == 0) {					// check barrier
		if(board[2*position.row][2*position.column-2] == 0) {				// check jump over opponent
			paint_square(position.row, position.column-1, color);			// show if no barrier and no opponent
			if(color == Green)																				// if we are picking a direction, then set the enum to that direction
				token_dir = Left;
		} else {																										// opponent is there
			if(position.column != 1) {																// the opponent is on the border, so you are in the second column
				if(board[2*position.row][2*position.column-3] == 0) {		// check barrier behind opponent
					paint_square(position.row, position.column-2, color);
					if(color == Green)
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
			if(color == Green)
				token_dir = Right;
		} else {
			if(position.column != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row][2*position.column+3] == 0) {
					paint_square(position.row, position.column+2, color);
					if(color == Green)
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
			if(color == Green)
				token_dir = Up;
		} else {
			if(position.row != 1) {
				if(board[2*position.row-3][2*position.column] == 0) {
					paint_square(position.row-2, position.column, color);
					if(color == Green)
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
			if(color == Green)
				token_dir = Down;
		} else {
			if(position.row != NUM_SQUARE_PER_SIDE-1) {
				if(board[2*position.row+3][2*position.column] == 0) {
					paint_square(position.row+2, position.column, color);
					if(color == Green)
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

uint8_t check_victory(Turn player_turn) {
	switch(player_turn) {
		case Player1:		// player_turn is the next player who will play, so it is the opposite to the who just made a move
			if(player2.row == 0)
				return 1;
			break;
		case Player2:
			if(player1.row == NUM_SQUARE_PER_SIDE-1)
				return 1;
			break;
	}
	return 0;
}

void routine_direction_picked(Pos_square* player, Direction dir, Turn player_turn) {
	Move move;
	
	move.old_row = player->row;
	move.old_column = player->column;
	
	Show_Possible_Moves(board, *player, White);					// get rid of possible moves
	paint_square(player->row, player->column, White);		// get rid of old position
	token_dir = Nothing;																// reset direction picked
	switch(dir) {
		case Up:
			if(board[2*player->row-1][2*player->column] == 0) {
				if(board[2*player->row-2][2*player->column] == 0) {
					player->row -= 1;
				} else {
					if(player->row != 1) {
						if(board[2*player->row-3][2*player->column] == 0) {
							player->row -= 2;
						}
					}
				}
			}
			break;
		case Down:
			if(board[2*player->row+1][2*player->column] == 0) {
				if(board[2*player->row+2][2*player->column] == 0) {
					player->row += 1;
				} else {
					if(player->row != NUM_SQUARE_PER_SIDE-1) {
						if(board[2*player->row+3][2*player->column] == 0) {
							player->row += 2;
						}
					}
				}
			}
			break;
		case Left:
			if(board[2*player->row][2*player->column-1] == 0) {					// check barrier
				if(board[2*player->row][2*player->column-2] == 0) {				// check jump over opponent
					player->column -= 1;
				} else {																										// opponent is there
					if(player->column != 1) {																// the opponent is on the border, so you are in the second column
						if(board[2*player->row][2*player->column-3] == 0) {		// check barrier behind opponent
							player->column -= 2;
						}
					}
				}
			}
			break;
		case Right:
			if(board[2*player->row][2*player->column+1] == 0) {
				if(board[2*player->row][2*player->column+2] == 0) {
					player->column += 1;
				} else {
					if(player->column != NUM_SQUARE_PER_SIDE-1) {
						if(board[2*player->row][2*player->column+3] == 0) {
							player->column += 2;
						}
					}
				}
			}
			break;
		default:
			break;
	}
	move.new_row = player->row;
	move.new_column = player->column;
	update_board_player_move(board, move);					// update board with move
	
	switch(turn) {
		case Player1:
			paint_square(player->row, player->column, Blue);	// paint new position
			break;
		case Player2:
			paint_square(player->row, player->column, Red);
			break;
	}
	
	timer_value = 20;																	// reset timer for next turn
	turn = player_turn;																// set new player turn
	swap = 1;																					// signal new turn
	
	victory = check_victory(player_turn);							// check for victory condition
}

void show_decision_square(Direction dir) {
	switch(dir) {			// switch for picking up joystick action
		case Up:
			switch(turn) {	// switch for player turn
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
					switch(token_dir) {		// switch for checking if a direction has been actually picked before confirming the end of the turn
						case Up:
							routine_direction_picked(&player1, Up, Player2);
							break;
						case Down:
							routine_direction_picked(&player1, Down, Player2);
							break;
						case Left:
							routine_direction_picked(&player1, Left, Player2);
							break;
						case Right:
							routine_direction_picked(&player1, Right, Player2);
							break;
						default:			// no valid direction picked yet, pressing the joystick does nothing
							break;
					}
					break;
				case Player2:
					Show_Possible_Moves(board, player2, White);
					switch(token_dir) {
						case Up:
							routine_direction_picked(&player2, Up, Player1);
							break;
						case Down:
							routine_direction_picked(&player2, Down, Player1);
							break;
						case Left:
							routine_direction_picked(&player2, Left, Player1);
							break;
						case Right:
							routine_direction_picked(&player2, Right, Player1);
							break;
						default:
							break;
					}
					break;
			}
			break;
		default:
			break;
	}
}

void new_turn(void) {
	switch(turn){
		case Player1:
			Show_Possible_Moves(board, player1, Yellow);
			break;
		case Player2:
			Show_Possible_Moves(board, player2, Yellow);
			break;
	}
}

void Start_Game(void) {
	Move init_p1 = {0,3,0,3};
	Move init_p2 = {6,3,6,3};

	init_timer(0, 0x004C4B40);
	enable_timer(0);

	update_board_player_move(board, init_p1);
	update_board_player_move(board, init_p2);
	
	Show_Possible_Moves(board, player1, Yellow);
}
