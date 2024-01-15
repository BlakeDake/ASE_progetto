#include "game_logic.h"

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


uint8_t swap = 0;
uint8_t victory = 0;
Direction token_dir = Nothing;
uint8_t board[BOARD_LENGTH][BOARD_LENGTH] = {0};
Pos_square player1 = {0,3};
Pos_square player2 = {6,3};
Turn turn = Player1;
Mode mode = Token;
Pos_barrier moving_barrier = {2,2,0};
uint8_t p1_walls = 8;
uint8_t p2_walls = 8;
Graph g;

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
			board[2*barrier.row+1][2*barrier.column+1] = 2;			// 2 signals that the wall is present it is horizontal
			board[2*barrier.row+1][2*barrier.column+2] = 1;
			paint_barrier(barrier.row, barrier.column, barrier.direction, Magenta);
			break;
		case 1:			// vertical
			board[2*barrier.row][2*barrier.column+1] = 1;
			board[2*barrier.row+1][2*barrier.column+1] = 3;		// 3 signals that the wall is present it is vertical
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
	switch(dir) {					// same calculations as in the paint_dir_square functions to get where you actually move to
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

	swap = 1;																					// signal new turn
	
	victory = check_victory(player_turn);							// check for victory condition
}

void show_decision_square(Direction dir) {
	switch(dir) {			// switch for picking up joystick action
		case Up:
			switch(turn) {	// switch for player turn
				case Player1:
					if(player1.row != 0) {
						Show_Possible_Moves(board, player1, Yellow);
						paint_up_square(board, player1, Green);
					}
					break;
				case Player2:
					if(player2.row != 0) {
						Show_Possible_Moves(board, player2, Yellow);
						paint_up_square(board, player2, Green);
					}
					break;
			}
			break;
		case Down:
			switch(turn) {
				case Player1:
					if(player1.row != NUM_SQUARE_PER_SIDE-1) {
						Show_Possible_Moves(board, player1, Yellow);
						paint_down_square(board, player1, Green);
					}
					break;
				case Player2:
					if(player2.row != NUM_SQUARE_PER_SIDE-1) {
						Show_Possible_Moves(board, player2, Yellow);
						paint_down_square(board, player2, Green);
					}
					break;
			}
			break;
		case Left:
			switch(turn) {
				case Player1:
					if(player1.column != 0) {
						Show_Possible_Moves(board, player1, Yellow);
						paint_left_square(board, player1, Green);
					}
					break;
				case Player2:
					if(player2.column != 0) {
						Show_Possible_Moves(board, player2, Yellow);
						paint_left_square(board, player2, Green);
					}
					break;
			}
			break;
		case Right:
			switch(turn) {
				case Player1:
					if(player1.column != NUM_SQUARE_PER_SIDE-1) {
						Show_Possible_Moves(board, player1, Yellow);
						paint_right_square(board, player1, Green);
					}
					break;
				case Player2:
					if(player1.column != NUM_SQUARE_PER_SIDE-1) {
						Show_Possible_Moves(board, player2, Yellow);
						paint_right_square(board, player2, Green);
					}
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

void show_wall_center(uint8_t board[][BOARD_LENGTH], uint16_t color) {
	paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, color);
}

void repaint_existing_walls(Pos_barrier barrier) {
		switch(board[2*barrier.row+1][2*barrier.column+1]) {
			case 0:		// no barrier, I simply paint a white barrier with the same direction
				if(barrier.row == moving_barrier.row && barrier.column == moving_barrier.column) {
					// check if the barrier as param is the same as moving_barrier, this points to if we are painting out the moving barrier or repainting barriers that have been covered
					paint_barrier(barrier.row, barrier.column, barrier.direction, White);
				}
				break;
			case 2:		// horizontal barrier, I paint first a vertical white barrier, then the actual barrier
				if(barrier.row == moving_barrier.row && barrier.column == moving_barrier.column) {
					paint_barrier(barrier.row, barrier.column, 1, White);
					paint_barrier(barrier.row, barrier.column, 0, Magenta);
				} else {
					paint_barrier(barrier.row, barrier.column, 0, Magenta);
				}
				break;
			case 3:		// vertical barrier
				if(barrier.row == moving_barrier.row && barrier.column == moving_barrier.column) {
					paint_barrier(barrier.row, barrier.column, 0, White);
					paint_barrier(barrier.row, barrier.column, 1, Magenta);
				} else {
					paint_barrier(barrier.row, barrier.column, 1, Magenta);
				}
				break;
			default:
				break;
		}
}
void repaint_existing_walls_handler() {
	Pos_barrier tmp_barrier = moving_barrier;

	repaint_existing_walls(moving_barrier);										// repaint barriers in the actual position

	if(moving_barrier.row != 0) {
		tmp_barrier = moving_barrier;
		tmp_barrier.row -= 1;
		repaint_existing_walls(tmp_barrier);
	}
	if(moving_barrier.row != WALL_SLOTS_PER_SIDE-1) {
		tmp_barrier = moving_barrier;
		tmp_barrier.row += 1;
		repaint_existing_walls(tmp_barrier);
	}
	if(moving_barrier.column != 0) {
		tmp_barrier = moving_barrier;
		tmp_barrier.column -= 1;
		repaint_existing_walls(tmp_barrier);
	}
	if(moving_barrier.column != WALL_SLOTS_PER_SIDE-1) {
		tmp_barrier = moving_barrier;
		tmp_barrier.column += 1;
		repaint_existing_walls(tmp_barrier);
	}
}

void paint_up_wall(void) {
	if(moving_barrier.row != 0) {
		repaint_existing_walls_handler();
		moving_barrier.row -= 1;
		paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
	}
}

void paint_down_wall(void) {
	if(moving_barrier.row != WALL_SLOTS_PER_SIDE-1) {
		repaint_existing_walls_handler();
		moving_barrier.row += 1;
		paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
	}
}

void paint_right_wall(void) {
	if(moving_barrier.column != WALL_SLOTS_PER_SIDE-1) {
		repaint_existing_walls_handler();
		moving_barrier.column += 1;
		paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
	}
}

void paint_left_wall(void) {
	if(moving_barrier.column != 0) {
		repaint_existing_walls_handler();
		moving_barrier.column -= 1;
		paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
	}
}

void paint_rotate_wall(void) {
	switch(moving_barrier.direction) {
		case 0:		// from horizontal to vertical
			repaint_existing_walls_handler();
			moving_barrier.direction = 1;
			paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
			break;
		case 1:		// from vertical to horizontal
			repaint_existing_walls_handler();
			moving_barrier.direction = 0;
			paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Cyan);
			break;
		default:
			break;
	}
}

uint8_t check_wall_overlap() {
	uint8_t check = 1;
	switch(moving_barrier.direction) {
		case 0:			// horizontal
			if(board[2*moving_barrier.row+1][2*moving_barrier.column+1] != 0) check = 0;					// no barrier at the actual spot
			if(moving_barrier.column != 0) 																												// no horizontal barrier on the left
				if(board[2*moving_barrier.row+1][2*moving_barrier.column-1] == 2) check = 0;	
			if(moving_barrier.column != WALL_SLOTS_PER_SIDE-1) 																		// no horizontal barrier on the right
				if(board[2*moving_barrier.row+1][2*moving_barrier.column+3] == 2) check = 0;
			break;
		case 1:			// vertical
			if(board[2*moving_barrier.row+1][2*moving_barrier.column+1] != 0) check = 0;					// no barrier at the actual spot
			if(moving_barrier.column != 0) 																												// no vertical barrier above
				if(board[2*moving_barrier.row-1][2*moving_barrier.column+1] == 3) check = 0;
			if(moving_barrier.column != WALL_SLOTS_PER_SIDE-1)  																	// no vertical barrier underneath
				if(board[2*moving_barrier.row+3][2*moving_barrier.column+1] == 3)	check = 0;
			break;
	}
	return check;
}
uint8_t check_pathability() {
	int top_left = moving_barrier.row*NUM_SQUARE_PER_SIDE + moving_barrier.column;
	int top_right = moving_barrier.row*NUM_SQUARE_PER_SIDE + moving_barrier.column + 1;
	int bottom_left = (moving_barrier.row+1)*NUM_SQUARE_PER_SIDE + moving_barrier.column;
	int bottom_right = (moving_barrier.row+1)*NUM_SQUARE_PER_SIDE + moving_barrier.column + 1;
	uint8_t i;
	bool flag1 = false;
	bool flag2 = false;

	int start1 = player1.row*NUM_SQUARE_PER_SIDE + player1.column;
	int end1 = (NUM_SQUARE_PER_SIDE-1)*NUM_SQUARE_PER_SIDE;
	int start2 = player2.row*NUM_SQUARE_PER_SIDE + player2.column;
	int end2 = 0;

	switch(moving_barrier.direction) {		// delete temporarily the two edges
		case 0:				 // horizontal
			delete_edge(&g, top_left, bottom_left);
			delete_edge(&g, top_right, bottom_right);

			for(i = 0; i < NUM_SQUARE_PER_SIDE && flag1 == false && flag2 == false; i++) {
				if(path_exists(&g, start1, end1+i)) {
					flag1 = true;
				}
				if(path_exists(&g, start2, end2+i)) {
					flag2 = true;
				}
			}

			if(!flag1 || !flag2) {
				add_edge(&g, top_left, bottom_left);
				add_edge(&g, top_right, bottom_right);
			} else {
				return 1;
			}
			break;
		case 1:					// vertical
			delete_edge(&g, top_left, top_right);
			delete_edge(&g, bottom_left, bottom_right);

			for(i = 0; i < NUM_SQUARE_PER_SIDE && flag1 == false && flag2 == false; i++) {
				if(path_exists(&g, start1, end1+i)) {
					flag1 = true;
				}
				if(path_exists(&g, start2, end2+i)) {
					flag2 = true;
				}
			}

			if(!flag1 || !flag2) {
				add_edge(&g, top_left, top_right);
				add_edge(&g, bottom_left, bottom_right);
			} else {
				return 1;
			}
			break;
		default:
			break;
	}
	return 0;
}
uint8_t check_if_wall_is_placeable() {
	if(check_wall_overlap()) {
		return check_pathability();
	}
	return 0;
}

void confirm_wall_placement() {
	paint_barrier(moving_barrier.row, moving_barrier.column, moving_barrier.direction, Magenta);
	update_board_barrier(board, moving_barrier);
	moving_barrier.row = 2;
	moving_barrier.column = 2;
	moving_barrier.direction = 0;
}

uint8_t show_wall_movement(Wall_Direction dir) {
	char buffer[2];

	switch(dir) {
		case Wall_Up:
			paint_up_wall();
			break;
		case Wall_Down:
			paint_down_wall();
			break;
		case Wall_Left:
			paint_left_wall();
			break;
		case Wall_Right:
			paint_right_wall();
			break;
		case Wall_Select:
			if(check_if_wall_is_placeable()) {
				confirm_wall_placement();
				switch(turn) {
					case Player1:
						p1_walls--;
						sprintf(buffer, "%d", p1_walls);
						GUI_Text(SIDE_RECTANGLE_GAP+2,
										 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2+12,
										 (uint8_t *)buffer,
										 Black,
										 White);
						break;
					case Player2:
						p2_walls--;
						sprintf(buffer, "%d", p2_walls);
						GUI_Text(SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+RECTANGLE_HORIZONTAL_SIDE_LENGTH+SIDE_RECTANGLE_GAP+2,
										 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+UP_UNDER_RECTANGLE_GAP+2+12,
										 (uint8_t *)buffer,
										 Black,
										 White);
						break;
				}
				return 1;
			} else {
				return 0;
			}
			break;
		case Wall_Rotate:
			paint_rotate_wall();
			break;
		default:
			break;
	}
}

void reset_moving_barrier() {
	moving_barrier.row = 2;
	moving_barrier.column = 2;
	moving_barrier.direction = 0;
}

void routine_mode(void) {
	switch(mode) {			// old mode
		case Token:				// going from token to walls
			switch(turn) {
				case Player1:
					if(p1_walls) {
						Show_Possible_Moves(board, player1, White);
						show_wall_center(board, Cyan);
						mode = Wall;
					} else {
						GUI_Text(SIDE_RECTANGLE_GAP+2,
										 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
										 "No more walls!",
										 Black,
										 White);
					}
					break;
				case Player2:
					if(p2_walls) {
						Show_Possible_Moves(board, player2, White);
						show_wall_center(board, Cyan);
						mode = Wall;
					} else {
						GUI_Text(SIDE_RECTANGLE_GAP+2,
										 GRID_BORDER_GAP+(NUM_SQUARE_PER_SIDE*SQUARE_SIDE_LENGTH)+(6*SQUARE_GAP)+10,
										 "No more walls!",
										 Black,
										 White);
					}
					break;
			}
			break;
		case Wall:			// going from walls to token
			switch(turn) {
				case Player1:
					repaint_existing_walls_handler();
					reset_moving_barrier();
					Show_Possible_Moves(board, player1, Yellow);
					break;
				case Player2:
					repaint_existing_walls_handler();
					reset_moving_barrier();
					Show_Possible_Moves(board, player2, Yellow);
					break;
			}
			mode = Token;
			break;
	}
}

void new_turn(void) {
	mode = Token;
	timer_value = 20;
	repaint_existing_walls_handler();
	reset_moving_barrier();
	switch(turn) {
		case Player1:				// turn pass to Player2
			Show_Possible_Moves(board, player1, White);
			turn = Player2;
			Show_Possible_Moves(board, player2, Yellow);
			break;
		case Player2:				// turn pass to Player1
			Show_Possible_Moves(board, player2, White);
			turn = Player1;
			Show_Possible_Moves(board, player1, Yellow);
			break;
	}
}

void init_graph_routine(void) {
	init_graph(&g, MAX_NODES);
	create_board_graph(&g);
}

void Start_Game(void) {
	Move init_p1 = {0,3,0,3};
	Move init_p2 = {6,3,6,3};

	init_timer(0, 0x017D7840);		// 1 sec irl
	enable_timer(0);

	update_board_player_move(board, init_p1);
	update_board_player_move(board, init_p2);
	
	init_graph_routine();
	
	Show_Possible_Moves(board, player1, Yellow);
}
