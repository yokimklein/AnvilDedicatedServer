#include "player_mapping.h"

long __fastcall player_mapping_get_next_output_user(short absolute_player_index, long user_index)
{
	return INVOKE(0xE1F90, player_mapping_get_next_output_user, absolute_player_index, user_index);
}