#pragma once
#include <game\game_variant.h>
#include <scenario\scenario_map_variant.h>
#include <interface\user_interface_networking.h>

FUNCTION_DEF(0x3ABEC0, bool, __fastcall, user_interface_squad_set_game_variant, c_game_variant* game_variant);
FUNCTION_DEF(0x3ABC60, bool, __fastcall, user_interface_squad_set_multiplayer_map, c_map_variant* map_variant);
FUNCTION_DEF(0x3AA7D0, void, __fastcall, user_interface_set_desired_multiplayer_mode, e_desired_multiplayer_mode multiplayer_mode);