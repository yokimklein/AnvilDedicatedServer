#include "game_engine_scoring.h"

void game_engine_scoring_notify_statborg_reset()
{
	// TODO: structs for these
	*(long*)BASE_ADDRESS(0x3FDC928) = NONE;
	*(long*)BASE_ADDRESS(0x3FDC9A8) = NONE;
	*(long*)BASE_ADDRESS(0x3FDCA28) = NONE;
}