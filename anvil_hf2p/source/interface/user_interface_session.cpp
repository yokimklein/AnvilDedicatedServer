#include "user_interface_session.h"
#include <cseries\cseries.h>
#include <networking\logic\network_join.h>

bool __fastcall user_interface_squad_set_game_variant(c_game_variant* game_variant)
{
	return INVOKE(ADDRESS_USER_INTERFACE_SQUAD_SET_GAME_VARIANT, user_interface_squad_set_game_variant, game_variant);
}

bool __fastcall user_interface_squad_set_multiplayer_map(c_map_variant* map_variant)
{
	return INVOKE(ADDRESS_USER_INTERFACE_SQUAD_SET_MULTIPLAYER_MAP, user_interface_squad_set_multiplayer_map, map_variant);
}

void __fastcall user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode multiplayer_mode)
{
	INVOKE(ADDRESS_USER_INTERFACE_SET_DESIRED_MULTIPLAYER_MODE, user_interface_set_desired_multiplayer_mode, multiplayer_mode);
}

void user_interface_join_squad_abort()
{
	network_join_squad_join_abort();
}
