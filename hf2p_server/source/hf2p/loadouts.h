#pragma once
#include <memory\data.h>

//struct s_s3d_player_container;
//void __fastcall hf2p_set_ui_loadouts(qword* player_xuid, s_s3d_player_container* player_container);

struct player_datum;
void __fastcall player_update_loadout(datum_index player_index, player_datum* player_data);
// equipment_index from multiplayer_globals > Universal > Equipment
bool __fastcall equipment_add(long slot_index, long equipment_index);