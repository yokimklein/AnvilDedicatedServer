#include "loadouts.h"
#include <cseries\cseries.h>
#include <game\players.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <cache\cache_files.h>
#include <game\game_globals.h>
#include <hf2p\hf2p_definitions.h>
#include <game\multiplayer_definitions.h>
#include <tag_files\string_ids.h>
#include <anvil\backend\cache.h>

s_backend_loadout::s_backend_loadout()
{
	armour_suit = "armor_uninitialized";
	primary_weapon = "primary_weapon_uninitialized";
	secondary_weapon = "secondary_weapon_uninitialized";
	grenade = "grenade_uninitialized";
	booster.clear();
	for (long consumable_index = 0; consumable_index < NUMBEROF(consumables); consumable_index++)
	{
		consumables[consumable_index].clear();
	}
}

void s_backend_loadout::null_terminate_buffers()
{
	armour_suit.null_terminate_buffer();
	primary_weapon.null_terminate_buffer();
	secondary_weapon.null_terminate_buffer();
	grenade.null_terminate_buffer();
	booster.null_terminate_buffer();
	for (long consumable_index = 0; consumable_index < NUMBEROF(consumables); consumable_index++)
	{
		consumables[consumable_index].null_terminate_buffer();
	}
}

s_backend_customisation::s_backend_customisation()
{
	loadout_index = 0;
	account_label.clear();
	emblem.clear();
	colours[_armor_color_primary] = "color_primary_uninitialized";
	colours[_armor_color_secondary] = "color_secondary_uninitialized";
	colours[_armor_color_visor] = "color_visor_uninitialized";
	colours[_armor_color_lights] = "color_lights_uninitialized";
	colours[_armor_color_holo] = "color_holo_uninitialized";
}

void s_backend_customisation::null_terminate_buffers()
{
	account_label.null_terminate_buffer();
	emblem.null_terminate_buffer();
	for (long colour_index = 0; colour_index < NUMBEROF(colours); colour_index++)
	{
		colours[colour_index].null_terminate_buffer();
	}
}

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
	ASSERT(VALID_INDEX(slot_index, 4));
	return INVOKE(0xE0530, equipment_add, slot_index, equipment_index);
}

/* - These user storage related functions are from Saber's backend, which we've since disabled so these no longer work
// retrieves from cached loadouts
s_backend_loadout* user_get_loadout_from_api(qword user_xuid, long loadout_index)
{
	ASSERT(VALID_INDEX(loadout_index, k_maximum_loadouts));
	s_backend_loadout*(__cdecl**user_get_loadout_from_api_funcs)(qword) = (s_backend_loadout*(__cdecl**)(qword))base_address(0xEB09D4);
	return user_get_loadout_from_api_funcs[loadout_index](user_xuid);
}

// retrieves from cached customisations
s_backend_customisation* __cdecl user_get_customisation_from_api(qword user_xuid)
{
	return INVOKE(0xE0890, user_get_customisation_from_api, user_xuid);
}
*/
void s_backend_loadout::write_loadout(s_s3d_player_loadout* out_loadout, s_s3d_player_modifiers* out_modifiers)
{
	ASSERT(out_loadout);
	ASSERT(out_modifiers);
	// replaced saber's with our own
	//DECLFUNC(0x303CC0, void, __thiscall, s_backend_loadout*, s_s3d_player_loadout*)(this, out_loadout);

	// reset values
	csmemset(out_loadout, 0, sizeof(s_s3d_player_loadout));
	csmemset(out_modifiers, 0, sizeof(s_s3d_player_modifiers));

	out_loadout->gender = flags.test(0) ? _gender_female : _gender_male;

	s_cached_armor_item& armor_item = g_backend_data_cache.m_armor_items[armour_suit.get_string()];
	out_loadout->armor_suit = armor_item.gender_armor[out_loadout->gender];
	out_loadout->primary_weapon = g_backend_data_cache.m_weapons[primary_weapon.get_string()];
	out_loadout->secondary_weapon = g_backend_data_cache.m_weapons[secondary_weapon.get_string()];
	out_loadout->grenade = g_backend_data_cache.m_grenades[grenade.get_string()];
	out_loadout->support_pack = _support_package_none; // $TODO: I still have no idea if this does anything in game, maybe it's used for the loadout popups?
	for (long consumable_index = 0; consumable_index < NUMBEROF(out_loadout->consumables); consumable_index++)
	{
		out_loadout->consumables[consumable_index] = g_backend_data_cache.m_consumables[consumables[consumable_index].get_string()].consumable_index;
	}

	// $NOTE: I'm treating modifiers as additive if multiple of the same type are used, though maybe they originally only used the highest?
	for (s_modifier modifier : armor_item.modifiers)
	{
		out_modifiers->modifier_values[modifier.modifier] += modifier.value;
	}
	for (s_modifier modifier : g_backend_data_cache.m_boosters[booster.get_string()])
	{
		out_modifiers->modifier_values[modifier.modifier] += modifier.value;
	}
}

void s_backend_customisation::write_customisation(s_s3d_player_customization* out_customisation)
{
	ASSERT(out_customisation);
	// replaced saber's with our own
	//DECLFUNC(0x312110, void, __thiscall, s_backend_customisation*, ulong[k_armor_colors_count])(this, out_customisation->colors);

	for (long colour_index = 0; colour_index < k_armor_colors_count; colour_index++)
	{
		out_customisation->colors[colour_index] = g_backend_data_cache.m_colours[colours[colour_index].get_string()];
	}
	out_customisation->active_loadout_index = static_cast<byte>(loadout_index);
}
