#include "timer/timer.h"
#include "coloring.h"

typedef enum Direction {
	Nothing,
	Up,
	Down,
	Right,
	Left,
	Select
} Direction;

extern uint8_t swap;

void Start_Game(void);
void show_decision_square(Direction dir);
void new_turn(void);
