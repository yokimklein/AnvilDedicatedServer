#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_default.h>
#include <game\game_engine_sandbox_traits.h>
#include <saved_games\scenario_map_variant.h>

struct s_game_engine_sandbox_variant_definition;
class c_game_engine_sandbox_variant : public c_game_engine_base_variant
{
public:
	c_game_engine_sandbox_variant* constructor();

	void set(c_game_engine_sandbox_variant const* variant, bool force);
	void set(s_game_engine_sandbox_variant_definition const* definition);

	bool get_open_channel_voice() const;
	void set_open_channel_voice(bool open_channel_voice);

	e_sandbox_edit_mode_settings get_edit_mode() const;
	void set_edit_mode(e_sandbox_edit_mode_settings edit_mode);

	short get_respawn_time() const;
	void set_respawn_time(short respawn_time);

	c_player_traits* get_player_traits_writeable();
	c_player_traits const* get_player_traits() const;
	void set_player_traits(c_player_traits const* traits, bool force);

protected:
	c_flags<e_sandbox_variant_flags, byte_flags, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_edit_mode_settings, char, _sandbox_edit_mode_settings_all_players, k_sandbox_edit_mode_settings> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, _sandbox_respawn_time_instant, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_player_traits;
};
static_assert(sizeof(c_game_engine_sandbox_variant) == 0x1F0);

struct c_sandbox_engine : c_game_engine
{
};

struct s_core_quota
{
	long object_definition_index;
	byte maximum_count;
	byte minimum_count;
	byte placed_on_map;
	byte pad0;
};
static_assert(sizeof(s_core_quota) == 0x8);

struct s_sandbox_globals
{
	s_core_quota quota;
	long player_role_bit_array;
	c_static_array<long, 16> edited_object_ids;
	c_static_array<float, 16> cursor_distance_from_camera;
	c_static_array<real_point3d, 16> cursor_position;
	c_static_array<real_vector3d, 16> cursor_normal;
	c_static_array<real_vector3d, 16> cursor_vector;
	c_static_array<long, 16> object_under_cursor;
	c_static_array<bool, 16> rotating_object;
	c_static_array<long, 64> tracked_edited_objects;
	c_static_array<real_point3d, 64> tracked_edited_objects_last_soft_ceiling_position;
	long number_of_tracked_objects;
	long edited_map_variant_index;
	long current_object_definition_index;
	bool legal_accepted;
	long last_rendered_boundary_object_index;
	s_variant_multiplayer_object_properties_definition edited_object_properties;
};
static_assert(sizeof(s_sandbox_globals) == 0x748);