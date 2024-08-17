#pragma once
#include <memory\data.h>
#include <game\player_appearance.h>

constexpr long k_maximum_loadouts = 3;

struct s_api_user_loadout
{
	void write_configuration(s_s3d_player_loadout* out_loadout);

	long gender;
	char armour_suit[32];
	char primary_weapon[32];
	char secondary_weapon[32];
	char grenade[32];
	char support_pack[32];
	char tactical_pack1[32];
	char tactical_pack2[32];
	char tactical_pack3[32];
	char tactical_pack4[32];
};
static_assert(sizeof(s_api_user_loadout) == 0x124);

struct s_api_user_customisation
{
	void write_colours(s_s3d_player_customization* out_customisation);

	long loadout_index;
	char account_label[32];
	char emblem[32];
	char primary_colour[32];
	char secondary_colour[32];
	char visor_colour[32];
	char lights_colour[32];
	char hologram_colour[32];
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