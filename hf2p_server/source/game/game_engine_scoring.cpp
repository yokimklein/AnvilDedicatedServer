#include "game_engine_scoring.h"

void game_engine_scoring_notify_statborg_reset()
{
	*(long*)base_address(0x3FDC928) = -1;
	*(long*)base_address(0x3FDC9A8) = -1;
	*(long*)base_address(0x3FDCA28) = -1;
}