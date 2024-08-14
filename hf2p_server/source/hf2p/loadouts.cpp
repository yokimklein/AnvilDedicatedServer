#include "loadouts.h"
#include <cseries\cseries.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_engine_player.h>

//void __fastcall hf2p_set_ui_loadouts(qword* player_xuid, s_s3d_player_container* player_container)
//{
//	INVOKE(0x304390, hf2p_set_ui_loadouts, player_xuid, player_container);
//}

void __fastcall player_update_loadout(datum_index player_index, player_datum* player_data)
{
	byte active_loadout = player_data->configuration.client.active_loadout;
	long character_active_index = player_data->configuration.host.s3d_player_customization.character_active_index;
	if (active_loadout < 3)
	{
		character_active_index = active_loadout;
	}

	if (character_active_index != player_data->character_type_index)
	{
		qword user_xuid = player_data->configuration.host.user_xuid;
		player_data->revenge_shield_boost_multiplier = 0;
		player_data->character_type_index = character_active_index;
		DECLFUNC(0xE05E0, void, __cdecl, qword user_xuid)(user_xuid);
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_character_type);
	}
}

bool __fastcall equipment_add(long slot_index, long equipment_index)
{
	assert(VALID_INDEX(slot_index, 4));
	return INVOKE(0xE0530, equipment_add, slot_index, equipment_index);
}