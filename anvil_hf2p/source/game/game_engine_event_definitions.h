#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <tag_files\tag_groups.h>
#include <cseries\language.h>

enum e_multiplayer_event_type
{
	_multiplayer_event_type_general = 0,
	_multiplayer_event_type_flavor,
	_multiplayer_event_type_slayer,
	_multiplayer_event_type_capture_the_flag,
	_multiplayer_event_type_oddball,
	_multiplayer_event_type_dummy,
	_multiplayer_event_type_king_of_the_hill,
	_multiplayer_event_type_vip,
	_multiplayer_event_type_territories,
	_multiplayer_event_type_juggernaut,
	_multiplayer_event_type_assault,
	_multiplayer_event_type_infection,
	_multiplayer_event_type_survival,
	_multiplayer_event_type_earn_wp, // saber xp scoring

	k_multiplayer_event_type_count
};

enum e_game_engine_event_flags
{
	_game_engine_event_quantity_message_bit = 0,
	_game_engine_event_suppress_text_bit,

	k_game_engine_event_flags
};

enum e_game_engine_event_audience
{
	_game_engine_event_audience_cause_player = 0,
	_game_engine_event_audience_cause_team,
	_game_engine_event_audience_effect_player,
	_game_engine_event_audience_effect_team,
	_game_engine_event_audience_all,

	k_game_engine_event_audience_count
};

enum e_game_engine_event_response_context
{
	_game_engine_event_response_context_self = 0,
	_game_engine_event_response_context_friendly,
	_game_engine_event_response_context_enemy,
	_game_engine_event_response_context_neutral,

	k_game_engine_event_response_context_count
};

enum e_game_engine_event_input
{
	_game_engine_event_input_none = 0,
	_game_engine_event_input_cause_player,
	_game_engine_event_input_cause_team,
	_game_engine_event_input_effect_player,
	_game_engine_event_input_effect_team,

	k_game_engine_event_input_count
};

enum e_game_engine_event_splitscreen_suppression
{
	_game_engine_event_splitscreen_suppression_none = 0,
	_game_engine_event_splitscreen_suppression_suppress_audio,
	_game_engine_event_splitscreen_suppression_suppress_audio_if_overlapping,
	_game_engine_event_splitscreen_suppression_suppress_text,
	_game_engine_event_splitscreen_suppression_suppress_audio_and_text,

	k_game_engine_event_splitscreen_suppression_count
};

enum e_game_engine_sound_response_flags
{
	_game_engine_sound_response_announcer_sound_bit,

	k_game_engine_sound_response_flags
};

struct s_territories_event_data
{
	short territory_index;
};
static_assert(sizeof(s_territories_event_data) == 0x2);

struct s_objective_game_role_change_event_data
{
	long objective_game_role_index;
};
static_assert(sizeof(s_objective_game_role_change_event_data) == 0x4);

struct s_game_engine_event_data
{
	c_enum<e_multiplayer_event_type, long, _multiplayer_event_type_general, k_multiplayer_event_type_count> event_type;
	string_id event_name;
	long event_identifier;
	datum_index audience_player_index;
	datum_index cause_player_index;
	long cause_team_index;
	datum_index effect_player_index;
	long effect_team_index;
	long event_quantity;

	union
	{
		s_territories_event_data territories_event_data;
		s_objective_game_role_change_event_data objective_game_role_event_data;
	};
};
static_assert(sizeof(s_game_engine_event_data) == 0x28);

struct s_multiplayer_event_sound_response_definition
{
	c_flags<e_game_engine_sound_response_flags, word, k_game_engine_sound_response_flags> flags;
	short pad;
	c_typed_tag_reference<SOUND_TAG> sounds[k_language_count];
	float probability;
};
static_assert(sizeof(s_multiplayer_event_sound_response_definition) == 0xC8);

struct s_multiplayer_event_response_definition
{
	c_flags<e_game_engine_event_flags, short, k_game_engine_event_flags> flags;
	short runtime_event_type;
	c_string_id event_id;
	c_static_string<256> title_instance_name;
	c_enum<e_game_engine_event_audience, short, _game_engine_event_audience_cause_player, k_game_engine_event_audience_count> audience;
	short display_priority;
	short sub_priority;
	c_enum<e_game_engine_event_response_context, short, _game_engine_event_response_context_self, k_game_engine_event_response_context_count> display_context;
	c_string_id display_string;
	c_string_id medal_award;
	short earned_wp;
	short pad;
	float display_time; // seconds
	c_enum<e_game_engine_event_input, short, _game_engine_event_input_none, k_game_engine_event_input_count> required_field;
	c_enum<e_game_engine_event_input, short, _game_engine_event_input_none, k_game_engine_event_input_count> excluded_audience;
	c_enum<e_game_engine_event_splitscreen_suppression, short, _game_engine_event_splitscreen_suppression_none, k_game_engine_event_splitscreen_suppression_count> splitscreen_suppression;
	short pad2;
	c_string_id primary_string;
	short primary_string_duration; // seconds
	c_string_id plural_display_string;
	float sound_delay_announcer_only;
	s_multiplayer_event_sound_response_definition default_sound;
	c_typed_tag_block<s_multiplayer_event_sound_response_definition> sound_permutations;
};
static_assert(sizeof(s_multiplayer_event_response_definition) == 0x20C);
static_assert(0x118 == OFFSETOF(s_multiplayer_event_response_definition, earned_wp));

void game_engine_initialize_event(e_multiplayer_event_type event_type, string_id event_name, s_game_engine_event_data* event_data);