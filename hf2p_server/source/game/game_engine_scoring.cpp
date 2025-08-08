#include "game_engine_scoring.h"

void game_engine_scoring_notify_statborg_reset()
{
	// TODO: structs for these
	*base_address<long*>(0x3FDC928) = NONE;
	*base_address<long*>(0x3FDC9A8) = NONE;
	*base_address<long*>(0x3FDCA28) = NONE;
}