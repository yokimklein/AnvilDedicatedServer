#include "loadouts.h"
#include <cseries\cseries.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_engine_player.h>

//void __fastcall hf2p_set_ui_loadouts(qword* player_xuid, s_s3d_player_container* player_container)
//{
//	INVOKE(0x304390, hf2p_set_ui_loadouts, player_xuid, player_container);
//}

void __fastcall player_update_loadout(datum_index player_index, player_datum* player)
{
	byte selected_loadout_index = player->configuration.client.selected_loadout_index;
	long desired_loadout_index = player->configuration.host.s3d_player_customization.active_loadout_index;
	if (selected_loadout_index < 3)
	{
		desired_loadout_index = selected_loadout_index;
	}
	if (desired_loadout_index != player->active_loadout_index)
	{
		qword user_xuid = player->configuration.host.user_xuid;
		player->revenge_shield_boost_multiplier = 0;
		player->active_loadout_index = desired_loadout_index;
		player_update_loadout_internal(desired_loadout_index, user_xuid);
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_loadout_index);
	}
}

#pragma runtime_checks("", off)
void __fastcall player_update_loadout_internal(long loadout_index, qword user_xuid)
{
	INVOKE(0xE05E0, player_update_loadout_internal, loadout_index, user_xuid);
	__asm add esp, 8; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

bool __fastcall equipment_add(long slot_index, long equipment_index)
{
	assert(VALID_INDEX(slot_index, 4));
	return INVOKE(0xE0530, equipment_add, slot_index, equipment_index);
}