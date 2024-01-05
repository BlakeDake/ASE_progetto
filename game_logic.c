#include "game_logic.h"
#include "timer/timer.h"


void Start_Game(void) {
	init_timer(0, 0x002DC6C0);
	enable_timer(0);
}
