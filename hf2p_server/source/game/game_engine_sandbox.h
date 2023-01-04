#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_default.h"
#include "game_engine_sandbox_traits.h"
#include "game_engine_variant.h"
#include "..\saved_games\scenario_map_variant.h"

class c_game_engine_sandbox_variant : c_game_engine_base_variant
{
public:
	c_game_engine_sandbox_variant() :
		m_variant_flags(),
		m_edit_mode(),
		m_respawn_time(),
		m_all_player_traits(),
		unused()
	{
	};

	c_flags<e_sandbox_variant_flags, byte_flags, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_editing_mode, char, k_sandbox_editing_mode> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_all_player_traits;

	byte unused[0x70];
};
static_assert(sizeof(c_game_engine_sandbox_variant) == 0x260);

struct c_sandbox_engine : c_game_engine
{
};

struct s_sandbox_globals
{
	dword __unknown0;
	byte maximum_count;
	byte minimum_count;
	bool placed_on_map;
	word player_mode_flags;
	dword held_objects[16];
	real held_object_distances[16];
	real_vector3d crosshair_points[16];
	real_vector3d crosshair_intersect_normals[16];
	real_vector3d crosshair_redirections[16];
	dword crosshair_objects[16];
	byte __unknown30C[16];
	dword __unknown31C[64];
	byte __data41C[768];
	dword __unknown71C;
	byte __unknown720[8];
	bool forge_legal_notice;
	dword __unknown72C;
	s_variant_multiplayer_object_properties_definition object_properties;
};
static_assert(sizeof(s_sandbox_globals) == 0x748);