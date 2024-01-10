#include "timer/timer.h"
#include "coloring.h"

enum direction {
	Up,
	Down,
	Right,
	Left
};

void Start_Game(void);
void show_decision_square(enum direction dir);
