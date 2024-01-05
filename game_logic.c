#include "game_logic.h"
#include "timer/timer.h"


void Start_Game(void) {
	init_timer(0, 0x00B71B00);
	enable_timer(0);
}
