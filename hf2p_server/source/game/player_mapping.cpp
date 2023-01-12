#include "player_mapping.h"
#include "..\dllmain.h"

long player_mapping_get_next_output_user(short absolute_player_index, long user_index)
{
	FUNCTION_DEF(0xE1F90, long, __fastcall, player_mapping_get_next_output_user_call, short absolute_player_index, long user_index);
	return player_mapping_get_next_output_user_call(absolute_player_index, user_index);
}