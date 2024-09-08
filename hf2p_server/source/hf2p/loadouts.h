#pragma once
#include <memory\data.h>
#include <game\player_appearance.h>

constexpr long k_maximum_loadouts = 3;

struct s_api_user_loadout
{
	void write_configuration(s_s3d_player_loadout* out_loadout);

	c_static_flags<32> flags;
	c_static_string<32> armour_suit;
	c_static_string<32> primary_weapon;
	c_static_string<32> secondary_weapon;
	c_static_string<32> grenade;
	c_static_string<32> support_pack;
	c_static_string<32> tactical_pack1;
	c_static_string<32> tactical_pack2;
	c_static_string<32> tactical_pack3;
	c_static_string<32> tactical_pack4;
};
static_assert(sizeof(s_api_user_loadout) == 0x124);

struct s_api_user_customisation
{
	void write_colours(s_s3d_player_customization* out_customisation);

	long loadout_index;
	c_static_string<32> account_label;
	c_static_string<32> emblem;
	c_static_string<32> primary_colour;
	c_static_string<32> secondary_colour;
	c_static_string<32> visor_colour;
	c_static_string<32> lights_colour;
	c_static_string<32> hologram_colour;
};
static_assert(sizeof(s_api_user_customisation) == 0xE4);

//struct s_s3d_player_container;
//void __fastcall hf2p_set_ui_loadouts(qword* player_xuid, s_s3d_player_container* player_container);
struct player_datum;
void __fastcall player_update_loadout(datum_index player_index, player_datum* player);
void __fastcall player_update_loadout_internal(long loadout_index, qword user_xuid);
// equipment_index from multiplayer_globals > Universal > Equipment
bool __fastcall equipment_add(long slot_index, long equipment_index);
s_api_user_loadout* user_get_loadout_from_api(qword user_xuid, long loadout_index);
s_api_user_customisation* __cdecl user_get_customisation_from_api(qword user_xuid);