#include "game_engine_scoring.h"

void game_engine_scoring_notify_statborg_reset()
{
	// TODO: structs for these
	*base_address<long*>(ADDRESS_SCORING_STATBORG_RESET_VALUE_0) = NONE;
	*base_address<long*>(ADDRESS_SCORING_STATBORG_RESET_VALUE_1) = NONE;
	*base_address<long*>(ADDRESS_SCORING_STATBORG_RESET_VALUE_2) = NONE;
}