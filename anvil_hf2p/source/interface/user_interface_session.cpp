#include "user_interface_session.h"
#include <cseries\cseries.h>

bool __fastcall user_interface_squad_set_game_variant(c_game_variant* game_variant)
{
	return INVOKE(0x3ABEC0, user_interface_squad_set_game_variant, game_variant);
}

bool __fastcall user_interface_squad_set_multiplayer_map(c_map_variant* map_variant)
{
	return INVOKE(0x3ABC60, user_interface_squad_set_multiplayer_map, map_variant);
}

void __fastcall user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode multiplayer_mode)
{
	INVOKE(0x3AA7D0, user_interface_set_desired_multiplayer_mode, multiplayer_mode);
}