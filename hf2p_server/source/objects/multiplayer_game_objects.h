#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_field.h>
#include <tag_files\tag_groups.h>

enum e_multiplayer_object_type
{
	_object_multiplayer_type_ordinary,
	_object_multiplayer_type_weapon,
	_object_multiplayer_type_grenade,
	_object_multiplayer_type_projectile,
	_object_multiplayer_type_powerup,
	_object_multiplayer_type_equipment,
	_object_multiplayer_type_light_land_vehicle,
	_object_multiplayer_type_heavy_land_vehicle,
	_object_multiplayer_type_flying_vehicle,
	_object_multiplayer_type_teleporter_2way,
	_object_multiplayer_type_teleporter_sender,
	_object_multiplayer_type_teleporter_receiver,
	_object_multiplayer_type_player_spawn_location,
	_object_multiplayer_type_player_respawn_zone,
	_object_multiplayer_type_hold_spawn_objective,
	_object_multiplayer_type_capture_spawn_objective,
	_object_multiplayer_type_hold_destination_objective,
	_object_multiplayer_type_capture_destination_objective,
	_object_multiplayer_type_hill_objective,
	_object_multiplayer_type_infection_haven_objective,
	_object_multiplayer_type_territory_objective,
	_object_multiplayer_type_vip_boundary_objective,
	_object_multiplayer_type_vip_destination_objective,
	_object_multiplayer_type_juggernaut_destination_objective,

	k_number_of_multiplayer_object_types
};

enum e_multiplayer_object_teleporter_flags
{
	_multiplayer_object_teleporter_disallow_players_bit,
	_multiplayer_object_teleporter_allow_light_land_vehicles_bit,
	_multiplayer_object_teleporter_allow_heavy_land_vehicles_bit,
	_multiplayer_object_teleporter_allow_flying_vehicles_bit,
	_multiplayer_object_teleporter_allow_projectiles_bit,

	k_number_of_multiplayer_object_teleporter_flags
};

enum e_multiplayer_object_flags
{
	_multiplayer_object_only_render_in_editor_bit,
	_multiplayer_object_valid_initial_player_spawn_bit,
	_multiplayer_object_fixed_boundary_orientation_bit,
	_multiplayer_object_inherit_owning_team_color_bit,
	_multiplayer_object_unknown4_bit,
	_multiplayer_object_unknown5_bit,
	_multiplayer_object_unknown6_bit,
	_multiplayer_object_unknown7_bit,
	_multiplayer_object_unknown8_bit,
	_multiplayer_object_unknown9_bit,
	_multiplayer_object_unknown10_bit,
	_multiplayer_object_unknown11_bit,
	_multiplayer_object_unknown12_bit,
	_multiplayer_object_unknown13_bit,
	_multiplayer_object_unknown14_bit,
	_multiplayer_object_unknown15_bit,

	k_number_of_multiplayer_object_flags
};

enum e_multiplayer_object_boundary_shape
{
	_multiplayer_object_boundary_shape_none,
	_multiplayer_object_boundary_shape_sphere,
	_multiplayer_object_boundary_shape_cylinder,
	_multiplayer_object_boundary_shape_box,

	k_number_of_multiplayer_object_boundary_shapes
};

enum e_multiplayer_object_spawn_timer_type
{
	_multiplayer_object_spawn_timer_starts_on_death,
	_multiplayer_object_spawn_timer_starts_on_disturbance,

	k_number_of_multiplayer_object_spawn_timer_types
};

enum e_game_engine_flags
{
	_game_engine_ctf_bit,
	_game_engine_slayer_bit,
	_game_engine_oddball_bit,
	_game_engine_koth_bit,
	_game_engine_sandbox_bit,
	_game_engine_vip_bit,
	_game_engine_juggernaut_bit,
	_game_engine_territories_bit,
	_game_engine_assault_bit,
	_game_engine_infection_bit,

	k_number_of_game_engine_flags
};

struct s_multiplayer_object_boundary_geometry_data
{
	long boundary_shape;

	// shader used for boundary geometry
	long standard_shader_or_opaque_shader_index;

	real boundary_width;
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
	real_matrix4x3 boundary_matrix;
	real boundary_raius;
};
static_assert(sizeof(s_multiplayer_object_boundary_geometry_data) == 0x50);

struct s_multiplayer_object_boundary_shader_definition
{
	c_typed_tag_reference<RENDER_METHOD_TAG> standard_shader;
	c_typed_tag_reference<RENDER_METHOD_TAG> opaque_shader;
};
static_assert(sizeof(s_multiplayer_object_boundary_shader_definition) == 0x20);

struct s_multiplayer_object_definition
{
	c_flags<e_game_engine_flags, word, k_number_of_game_engine_flags> engine_flags;
	c_enum<e_multiplayer_object_type, char, _object_multiplayer_type_ordinary, k_number_of_multiplayer_object_types> type;
	c_flags<e_multiplayer_object_teleporter_flags, byte, k_number_of_multiplayer_object_teleporter_flags> teleporter_passability;
	c_flags<e_multiplayer_object_flags, word, k_number_of_multiplayer_object_flags> flags;
	c_enum<e_multiplayer_object_boundary_shape, char, _multiplayer_object_boundary_shape_none, k_number_of_multiplayer_object_boundary_shapes> boundary_shape;
	c_enum<e_multiplayer_object_spawn_timer_type, char, _multiplayer_object_spawn_timer_starts_on_death, k_number_of_multiplayer_object_spawn_timer_types> spawn_timer_type;
	short default_spawn_time;
	short default_abandon_time;
	real boundary_width;
	real boundary_length;
	real boundary_top;
	real boundary_bottom;
	real unknown1;
	real unknown2;
	real unknown3;
	string_id boundary_center_marker;
	string_id spawned_object_marker_name;
	c_typed_tag_reference<OBJECT_TAG> spawned_object;
	string_id nyi_boundary_material;
	s_multiplayer_object_boundary_shader_definition boundary_shaders[k_number_of_multiplayer_object_boundary_shapes];
};
static_assert(sizeof(s_multiplayer_object_definition) == 0xC4);