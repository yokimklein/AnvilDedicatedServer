#include "loadouts.h"
#include <cseries\cseries.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <cache\cache_files.h>
#include <game\game_globals.h>
#include <hf2p\hf2p_definitions.h>
#include <game\multiplayer_definitions.h>
#include <tag_files\string_ids.h>

//void __fastcall hf2p_set_ui_loadouts(qword* player_xuid, s_s3d_player_container* player_container)
//{
//	INVOKE(0x304390, hf2p_set_ui_loadouts, player_xuid, player_container);
//}

void __fastcall player_update_loadout(datum_index player_index, player_datum* player)
{
	byte selected_loadout_index = player->configuration.client.selected_loadout_index;
	long desired_loadout_index = player->configuration.host.s3d_player_customization.active_loadout_index;
	if (selected_loadout_index < k_maximum_loadouts)
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

// retrieves from cached loadouts
s_api_user_loadout* user_get_loadout_from_api(qword user_xuid, long loadout_index)
{
	assert(VALID_INDEX(loadout_index, 3));
	s_api_user_loadout*(__cdecl**user_get_loadout_from_api_funcs)(qword) = (s_api_user_loadout*(__cdecl**)(qword))BASE_ADDRESS(0xEB09D4);
	return user_get_loadout_from_api_funcs[loadout_index](user_xuid);
}

// retrieves from cached customisations
s_api_user_customisation* __cdecl user_get_customisation_from_api(qword user_xuid)
{
	return INVOKE(0xE0890, user_get_customisation_from_api, user_xuid);
}

void s_api_user_loadout::write_configuration(s_s3d_player_loadout* out_loadout)
{
	assert(out_loadout);
	DECLFUNC(0x303CC0, void, __thiscall, s_api_user_loadout*, s_s3d_player_loadout*)(this, out_loadout);
}

void s_api_user_customisation::write_colours(s_s3d_player_customization* out_customisation)
{
	assert(out_customisation);
	DECLFUNC(0x312110, void, __thiscall, s_api_user_customisation*, ulong[k_armor_colors_count])(this, out_customisation->colors);
}