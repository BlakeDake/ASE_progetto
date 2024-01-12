#include "timer/timer.h"
#include "coloring.h"
#include <stdio.h>

typedef enum Direction {
	Nothing,
	Up,
	Down,
	Right,
	Left,
	Select
} Direction;

typedef enum Wall_Direction {
	Wall_Up,
	Wall_Down,
	Wall_Right,
	Wall_Left,
	Wall_Select,
	Wall_Rotate
} Wall_Direction;

typedef enum Mode {
	Token,
	Wall
} Mode;

typedef enum Turn {
	Player1,
	Player2
} Turn;

extern uint8_t swap;
extern Mode mode;
extern uint8_t victory;
extern Turn turn;

void Start_Game(void);
void show_decision_square(Direction dir);
void new_turn(void);
void routine_mode(void);
uint8_t show_wall_movement(Wall_Direction dir);
