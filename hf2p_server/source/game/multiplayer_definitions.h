#pragma once
#include <tag_files\tag_block.h>
#include <tag_files\tag_groups.h>
#include <game\game_engine_event_definitions.h>

struct s_multiplayer_universal_globals_definition;
struct s_multiplayer_runtime_globals_definition;
struct multiplayer_globals_definition
{
	static tag const k_group_tag = MULTIPLAYER_GLOBALS_TAG;

	c_typed_tag_block<s_multiplayer_universal_globals_definition> universal;
	c_typed_tag_block<s_multiplayer_runtime_globals_definition> runtime;
};
static_assert(sizeof(multiplayer_globals_definition) == 0x18);

struct s_multiplayer_energy_regeneration
{
	long duration;
	long energy_level;
};
static_assert(sizeof(s_multiplayer_energy_regeneration) == 0x8);

struct s_multiplayer_customized_model_character;
struct s_multiplayer_equipment;
struct s_multiplayer_weapon_selection;
struct s_multiplayer_vehicle_selection;
struct s_multiplayer_grenade_selection;
struct s_multiplayer_weapon_set;
struct s_multiplayer_vehicle_set;
struct s_multiplayer_podium_animation;
struct s_multiplayer_universal_globals_definition
{
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> random_player_names;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> team_names;
	c_typed_tag_block<s_multiplayer_equipment> equipment;
	c_typed_tag_block<s_multiplayer_energy_regeneration> energy_regeneration;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> multiplayer_text;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> sandbox_text;
	c_typed_tag_reference<SANDBOX_TEXT_VALUE_PAIR_DEFINITION_TAG> sandbox_object_properties_values;

	// Weapon Selections Halo3
	// Define the weapons you want to be available in multiplayer games here (does not include mp objects like ball or flag)
	c_typed_tag_block<s_multiplayer_weapon_selection> weapon_selections;

	// Vehicle Selections Halo3
	// Define the vehicles you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_selection> vehicle_selections;

	// Grenade Selections Halo3
	// Define the grenades you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_grenade_selection> grenade_selections;

	// Bomb Run Equipment HF2P - New field added since MS23
	c_typed_tag_reference<EQUIPMENT_TAG> hf2p_bomb_run;

	// Weapon Sets Halo3
	// Define the weapon sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_weapon_set> weapon_sets;

	// Vehicle Sets Halo3
	// Define the vehicle sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_set> vehicle_sets;

	c_typed_tag_block<s_multiplayer_podium_animation> podium_animations;

	// Game Engine Settings Halo3
	// This tag is only placeholder, until the Excel/XML setup system is up and running
	c_typed_tag_reference<GAME_ENGINE_SETTINGS_DEFINITION_TAG> halo3_game_engine_settings;
};
static_assert(sizeof(s_multiplayer_universal_globals_definition) == 0xD0);
static_assert(0x0 == OFFSETOF(s_multiplayer_universal_globals_definition, random_player_names));
static_assert(0x10 == OFFSETOF(s_multiplayer_universal_globals_definition, team_names));
static_assert(0x20 == OFFSETOF(s_multiplayer_universal_globals_definition, equipment));
static_assert(0x2C == OFFSETOF(s_multiplayer_universal_globals_definition, energy_regeneration));
static_assert(0x38 == OFFSETOF(s_multiplayer_universal_globals_definition, multiplayer_text));
static_assert(0x48 == OFFSETOF(s_multiplayer_universal_globals_definition, sandbox_text));
static_assert(0x58 == OFFSETOF(s_multiplayer_universal_globals_definition, sandbox_object_properties_values));
static_assert(0x68 == OFFSETOF(s_multiplayer_universal_globals_definition, weapon_selections));
static_assert(0x74 == OFFSETOF(s_multiplayer_universal_globals_definition, vehicle_selections));
static_assert(0x80 == OFFSETOF(s_multiplayer_universal_globals_definition, grenade_selections));
static_assert(0x8C == OFFSETOF(s_multiplayer_universal_globals_definition, hf2p_bomb_run));
static_assert(0x9C == OFFSETOF(s_multiplayer_universal_globals_definition, weapon_sets));
static_assert(0xA8 == OFFSETOF(s_multiplayer_universal_globals_definition, vehicle_sets));
static_assert(0xB4 == OFFSETOF(s_multiplayer_universal_globals_definition, podium_animations));
static_assert(0xC0 == OFFSETOF(s_multiplayer_universal_globals_definition, halo3_game_engine_settings));

template<tag ...k_group_tags>
struct s_multiplayer_globals_tag_reference : s_tag_reference
{
};

struct s_multiplayer_constants;
struct s_game_engine_status_response;
struct s_multiplayer_runtime_globals_definition
{
	c_typed_tag_reference<BIPED_TAG> editor_biped;
	c_typed_tag_reference<SCENERY_TAG> editor_helper;
	c_typed_tag_reference<WEAPON_TAG> flag;
	c_typed_tag_reference<WEAPON_TAG> ball;
	c_typed_tag_reference<WEAPON_TAG> assault_bomb;
	c_typed_tag_reference<CRATE_TAG> vip_influence_area;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> in_game_text;

	// gameplay modifier tags
	c_typed_tag_reference<PROJECTILE_TAG> plant_plasma_on_death_projectile;
	c_typed_tag_reference<EFFECT_TAG> vehicle_autoflip_effect;
	c_typed_tag_reference<EFFECT_TAG> safety_booster_effect;

	// `player_spawn`
	// int32 sound_index = player_has_gameplay_modifier(_gameplay_modifier_respawn_modifier) ? runtime_data->modifier_respawn_sound.index : runtime_data->default_respawn_sound.index
	c_typed_tag_reference<SOUND_TAG> default_respawn_sound;
	c_typed_tag_reference<SOUND_TAG> modifier_respawn_sound;

	c_typed_tag_block<s_multiplayer_globals_tag_reference<SOUND_TAG>> sounds;
	c_typed_tag_block<s_multiplayer_globals_tag_reference<SOUND_LOOPING_TAG>> looping_sounds;

	c_typed_tag_block<s_multiplayer_event_response_definition> earn_wp_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> general_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> flavor_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> slayer_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> ctf_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> oddball_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> king_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> vip_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> juggernaut_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> territories_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> assault_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> infection_events;

	long maximum_frag_count;
	long maximum_plasma_count;

	c_typed_tag_block<s_multiplayer_constants> multiplayer_constants;
	c_typed_tag_block<s_game_engine_status_response> state_responses;

	c_typed_tag_reference<BITMAP_TAG> scoreboard_emblem_bitmap;
	c_typed_tag_reference<BITMAP_TAG> scoreboard_dead_emblem_bitmap;

	c_typed_tag_reference<SHADER_HALOGRAM_TAG> hill_shader;

	c_typed_tag_reference<CHUD_DEFINITION_TAG> pregame_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> ctf_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> slayer_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> oddball_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> king_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> sandbox_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> vip_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> juggernaut_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> territories_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> assault_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG> infection_intro_message;

	// new since ms23
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> biped_interpolation_defaults;
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> vehicle_interpolation_defaults;
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> crate_interpolation_defaults;
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> item_interpolation_defaults;
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> projectile_interpolation_defaults;
	c_typed_tag_reference<SIMULATION_INTERPOLATION_TAG> object_interpolation_defaults;

	c_typed_tag_reference<SOUND_LOOPING_TAG> music_first_place;
	c_typed_tag_reference<SOUND_LOOPING_TAG> music_second_place;
	c_typed_tag_reference<SOUND_LOOPING_TAG> music_third_place;
	c_typed_tag_reference<SOUND_LOOPING_TAG> music_post_match;
};
static_assert(sizeof(s_multiplayer_runtime_globals_definition) == 0x308);
static_assert(0xD8 == OFFSETOF(s_multiplayer_runtime_globals_definition, earn_wp_events));

struct s_multiplayer_weapon_selection
{
	c_string_id name;

	// used only for random weapon set
	real random_weapon_set_weight;

	c_typed_tag_reference<WEAPON_TAG> weapon_tag;
};
static_assert(sizeof(s_multiplayer_weapon_selection) == 0x18);

struct s_multiplayer_object_remap_entry;
struct s_multiplayer_weapon_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_weapon_set) == 0x10);

struct s_multiplayer_vehicle_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_vehicle_set) == 0x10);

s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(multiplayer_globals_definition* multiplayer_globals);
string_id multiplayer_universal_data_get_weapon_selection_name_from_absolute_index(short absolute_index);
const s_multiplayer_weapon_set* multiplayer_universal_data_weapon_set_try_and_get(short absolute_index);
const s_multiplayer_vehicle_set* multiplayer_universal_data_vehicle_set_try_and_get(short absolute_index);
string_id multiplayer_universal_data_get_weapon_set_name_from_absolute_index(short absolute_index);
short multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(string_id name);
string_id multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(short absolute_index);
short multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(string_id name);