#pragma once
#include <cseries\cseries.h>
#include <objects\multiplayer_game_objects.h>

enum e_game_engine_symmetric_placement
{
	_game_engine_symmetric_placement_ignore = 0,
	_game_engine_symmetric_placement_symmetric,
	_game_engine_symmetric_placement_asymmetric,

	k_game_engine_symmetric_placement_count
};

enum e_scenario_game_engine
{
	_scenario_game_engine_ctf = 0,
	_scenario_game_engine_slayer,
	_scenario_game_engine_oddball,
	_scenario_game_engine_king,
	_scenario_game_engine_juggernaut,
	_scenario_game_engine_territories,
	_scenario_game_engine_assault,
	_scenario_game_engine_vip,
	_scenario_game_engine_infection,
	_scenario_game_engine_target_training,

	k_scenario_game_engine_count
};

enum e_multiplayer_team_designator
{
	_multiplayer_team_designator_defender = 0,
	_multiplayer_team_designator_attacker,
	_multiplayer_team_designator_third_party,
	_multiplayer_team_designator_fourth_party,
	_multiplayer_team_designator_fifth_party,
	_multiplayer_team_designator_sixth_party,
	_multiplayer_team_designator_seventh_party,
	_multiplayer_team_designator_eigth_party,
	_multiplayer_team_designator_neutral,

	k_multiplayer_team_designator_count
};

enum e_teleporter_channel
{
	_teleporter_channel_alpha,
	_teleporter_channel_bravo,
	_teleporter_channel_charlie,
	_teleporter_channel_delta,
	_teleporter_channel_echo,
	_teleporter_channel_foxtrot,
	_teleporter_channel_golf,
	_teleporter_channel_hotel,
	_teleporter_channel_india,
	_teleporter_channel_juliet,
	_teleporter_channel_kilo,
	_teleporter_channel_lima,
	_teleporter_channel_mike,
	_teleporter_channel_november,
	_teleporter_channel_oscar,
	_teleporter_channel_papa,
	_teleporter_channel_quebec,
	_teleporter_channel_romeo,
	_teleporter_channel_sierra,
	_teleporter_channel_tango,
	_teleporter_channel_uniform,
	_teleporter_channel_victor,
	_teleporter_channel_whiskey,
	_teleporter_channel_xray,
	_teleporter_channel_yankee,
	_teleporter_channel_zulu,

	k_teleporter_channel_count
};

struct s_variant_multiplayer_object_properties_definition
{
public:
	s_variant_multiplayer_object_properties_definition() :
		symmetry_placement_flags(),
		game_engine_flags()
	{
		initialize_to_default();
	}

	void initialize_to_default()
	{
		symmetry_placement_flags = _game_engine_symmetric_placement_ignore;
		game_engine_flags = 12;
		owner_team = 9;
		shared_storage = { 0 };
		spawn_rate = 0;
		teleporter_channel = _teleporter_channel_alpha;
		boundary_shape = _multiplayer_object_boundary_shape_none;
		boundary_radius = 0.0f;
		boundary_box_length = 0.0f;
		boundary_positive_height = 0.0f;
		boundary_negative_height = 0.0f;
	}

protected:
	c_flags<e_game_engine_symmetric_placement, word, k_game_engine_symmetric_placement_count> symmetry_placement_flags;
	c_flags<e_scenario_game_engine, byte, k_scenario_game_engine_count> game_engine_flags;
	c_enum<e_multiplayer_team_designator, byte, _multiplayer_team_designator_defender, k_multiplayer_team_designator_count> owner_team;

	union
	{
		char spare_clips;
		char teleporter_channel;
		char spawn_order;
	} shared_storage;

	// seconds
	char spawn_rate;

	c_enum<e_teleporter_channel, byte, _teleporter_channel_alpha, k_teleporter_channel_count> teleporter_channel;

	c_enum<e_multiplayer_object_boundary_shape, char, _multiplayer_object_boundary_shape_none, k_number_of_multiplayer_object_boundary_shapes> boundary_shape;

	union
	{
		real boundary_radius;
		real boundary_width;
	};
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
};
static_assert(sizeof(s_variant_multiplayer_object_properties_definition) == 0x18);