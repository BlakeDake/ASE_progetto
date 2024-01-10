#include "timer/timer.h"
#include "coloring.h"

typedef enum Direction {
	Up,
	Down,
	Right,
	Left,
	Select
} Direction;

void Start_Game(void);
void show_decision_square(Direction dir);
