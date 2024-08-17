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

s_api_user_loadout* user_get_loadout_from_api(qword user_xuid, long loadout_index)
{
	assert(VALID_INDEX(loadout_index, 3));
	s_api_user_loadout*(__cdecl**user_get_loadout_from_api_funcs)(qword) = (s_api_user_loadout*(__cdecl**)(qword))BASE_ADDRESS(0xEB09D4);
	return user_get_loadout_from_api_funcs[loadout_index](user_xuid);
}

s_api_user_customisation* __cdecl user_get_customisation_from_api(qword user_xuid)
{
	return INVOKE(0xE0890, user_get_customisation_from_api, user_xuid);
}

void s_api_user_loadout::write_configuration(s_s3d_player_loadout* out_loadout)
{
	out_loadout->gender = (byte)gender;

	long game_globals_index = cache_file_get_global_tag_index(GLOBALS_TAG);
	if (game_globals_index == NONE)
		return;

	s_game_globals* global_game_globals = (s_game_globals*)tag_get(GLOBALS_TAG, game_globals_index);
	assert(global_game_globals);

	hf2p_globals_definition* armor_globals = (hf2p_globals_definition*)tag_get(HF2P_GLOBALS_TAG, global_game_globals->armor_globals.index);
	assert(armor_globals);

	if (armor_globals->race_armors.count() < 0)
		return;
	if (armor_globals->race_armors[0].genders.count() != 2)
		return;
	
	// search armour globals tag for matching entry
	c_typed_tag_block<s_armor_objects> armor_objects = armor_globals->race_armors[0].genders[gender].armor_objects; // just spartans for now
	for (long i = 0; i < armor_objects.count(); i++)
	{
		if (strncmp(armor_objects[i].name.get_string(), armour_suit, 32) == 0)
		{
			out_loadout->armor_suit = i;
			break;
		}
	}

	// TODO: grab weapon ID from title instances instead, variants such as dmr_v2_type_1 don't have their own mulg entry
	multiplayer_globals_definition* multiplayer_globals = (multiplayer_globals_definition*)tag_get(MULTIPLAYER_GLOBALS_TAG, global_game_globals->multiplayer_globals.index);
	assert(multiplayer_globals);
	if (multiplayer_globals->universal.count() == 1)
	{
		// search multiplayer globals for matching weapons
		bool primary_found = false;
		bool secondary_found = false;
		for (long i = 0; i < multiplayer_globals->universal[0].weapon_selections.count(); i++)
		{
			const char* weapon_name = string_id_get_string_const(multiplayer_globals->universal[0].weapon_selections[i].name.get_value());
			if (csstrncmp(weapon_name, primary_weapon, 32) == 0)
			{
				out_loadout->primary_weapon = i;
				primary_found = true;
			}
			if (csstrncmp(weapon_name, secondary_weapon, 32) == 0)
			{
				out_loadout->secondary_weapon = i;
				secondary_found = true;
			}
			if (primary_found && secondary_found)
			{
				break;
			}
		}
	}

	// TODO: nades, tactical packs
}

void s_api_user_customisation::write_configuration(s_s3d_player_customization* out_customisation)
{

}