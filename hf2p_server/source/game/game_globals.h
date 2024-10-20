#pragma once
#include <game\game_options.h>
#include <game\game_progression.h>
#include <render\render_visibility_collection.h>
#include <tag_files\tag_block.h>
#include <tag_files\tag_groups.h>

// Non-official enum
enum e_game_difficulty_value_type
{
	// health
	_game_difficulty_value_enemy_damage,
	_game_difficulty_enemy_vitality,
	_game_difficulty_enemy_shield,
	_game_difficulty_enemy_recharge,
	_game_difficulty_friend_damage,
	_game_difficulty_friend_vitality,
	_game_difficulty_friend_shield,
	_game_difficulty_friend_recharge,
	_game_difficulty_infection_forms,
	_game_difficulty_pad1,

	// ranged fire
	_game_difficulty_rate_of_fire,
	_game_difficulty_projectile_error,
	_game_difficulty_burst_error,
	_game_difficulty_new_target_delay,
	_game_difficulty_burst_separation,
	_game_difficulty_target_tracking,
	_game_difficulty_target_leading,
	_game_difficulty_overcharge_chance,
	_game_difficulty_special_fire_delay,
	_game_difficulty_guidance_vs_player,
	_game_difficulty_melee_delay_base,
	_game_difficulty_melee_delay_scale,
	_game_difficulty_pad2,

	// grenades
	_game_difficulty_grenade_chance_scale,
	_game_difficulty_grenade_timer_scale,
	_game_difficulty_pad3,
	_game_difficulty_pad4,
	_game_difficulty_pad5,

	// placement
	_game_difficulty_major_upgrade_normal,
	_game_difficulty_major_upgrade_few,
	_game_difficulty_major_upgrade_many,

	// vehicles
	_game_difficulty_player_vehicle_ram_chance,
	_game_difficulty_pad6,
	_game_difficulty_pad7,
	_game_difficulty_pad8,

	NUMBER_OF_GAME_DIFFICULTY_VALUES
};

struct game_globals_storage
{
	bool initializing;
	bool map_active;
	byte : 8;
	byte : 8;

	dword active_structure_bsp_mask;
	dword active_designer_zone_mask;
	dword active_cinematic_zone_mask;

	game_options options;

	// ODST
	dword active_game_progression_level; // string id
	s_campaign_game_progression active_game_progression;

	bool game_in_progress;

	bool game_lost;
	bool game_revert;
	bool prepare_for_game_progression; // false if `active_game_progression_level` is `_invalid_string_id`
	long game_lost_wait_time;

	bool game_finished;
	byte : 8;
	byte : 8;
	byte : 8;
	long game_finished_wait_time;

	c_flags<e_campaign_skulls_primary, dword, k_campaign_skulls_primary_count> active_primary_skulls;
	c_flags<e_campaign_skulls_secondary, dword, k_campaign_skulls_secondary_count> active_secondary_skulls;

	byte : 8;

	// game_frame
	// game_had_an_update_tick_this_frame
	bool update_tick_this_frame;

	byte : 8;
	byte : 8;

	s_game_cluster_bit_vectors cluster_pvs;
	s_game_cluster_bit_vectors cluster_pvs_local;
	s_game_cluster_bit_vectors cluster_activation;

	// game_pvs_enable_scripted_camera_pvs
	// game_pvs_clear_scripted_camera_pvs
	bool scripted_camera_pvs;
	byte : 8;

	// game_pvs_scripted_clear
	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	word scripted;

	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	// scenario_group, scenario_block, scenario_cutscene_camera_point_block, camera_point_index
	datum_index scripted_object_index; // scenario_group.scenario_block.cutscene_camera_points[camera_point_index]

	long game_ragdoll_count;

	byte __data25204[4];
};
static_assert(sizeof(game_globals_storage) == 0x1A708);

struct s_game_globals_difficulty_information
{
	union
	{
		struct
		{
			// health
			// 
			// scale values for enemy health and damage settings
			real enemy_damage[k_number_of_campaign_difficulty_levels];    // enemy damage multiplier
			real enemy_vitality[k_number_of_campaign_difficulty_levels];  // enemy maximum body vitality scale
			real enemy_shield[k_number_of_campaign_difficulty_levels];    // enemy maximum shield vitality scale
			real enemy_recharge[k_number_of_campaign_difficulty_levels];  // enemy shield recharge scale
			real friend_damage[k_number_of_campaign_difficulty_levels];   // friend damage multiplier
			real friend_vitality[k_number_of_campaign_difficulty_levels]; // friend maximum body vitality scale
			real friend_shield[k_number_of_campaign_difficulty_levels];   // friend maximum shield vitality scale
			real friend_recharge[k_number_of_campaign_difficulty_levels]; // friend shield recharge scale
			real infection_forms[k_number_of_campaign_difficulty_levels]; // toughness of infection forms (may be negative)
			byte $[sizeof(real) * k_number_of_campaign_difficulty_levels];

			// ranged fire
			// 
			// difficulty-affecting values for enemy ranged combat settings
			real rate_of_fire[k_number_of_campaign_difficulty_levels];       // enemy rate of fire scale
			real projectile_error[k_number_of_campaign_difficulty_levels];   // enemy projectile error scale, as a fraction of their base firing error.
			real burst_error[k_number_of_campaign_difficulty_levels];        // enemy burst error scale; reduces intra-burst shot distance.
			real new_target_delay[k_number_of_campaign_difficulty_levels];   // enemy new-target delay scale factor.
			real burst_separation[k_number_of_campaign_difficulty_levels];   // delay time between bursts scale factor for enemies.
			real target_tracking[k_number_of_campaign_difficulty_levels];    // additional target tracking fraction for enemies.
			real target_leading[k_number_of_campaign_difficulty_levels];     // additional target leading fraction for enemies.
			real overcharge_chance[k_number_of_campaign_difficulty_levels];  // overcharge chance scale factor for enemies.
			real special_fire_delay[k_number_of_campaign_difficulty_levels]; // delay between special-fire shots (overcharge, banshee bombs) scale factor for enemies.
			real guidance_vs_player[k_number_of_campaign_difficulty_levels]; // guidance velocity scale factor for all projectiles targeted on a player.
			real melee_delay_base[k_number_of_campaign_difficulty_levels];   // delay period added to all melee attacks, even when berserk.
			real melee_delay_scale[k_number_of_campaign_difficulty_levels];  // multiplier for all existing non-berserk melee delay times.
			byte $$[sizeof(real) * k_number_of_campaign_difficulty_levels];

			// grenades
			// 
			// difficulty-affecting values for enemy grenade behavior
			real grenade_chance_scale[k_number_of_campaign_difficulty_levels]; // scale factor affecting the desicions to throw a grenade.
			real grenade_timer_scale[k_number_of_campaign_difficulty_levels];  // scale factor affecting the delay period between grenades thrown from the same encounter (lower is more often).
			byte $$$[sizeof(real) * k_number_of_campaign_difficulty_levels];
			byte $$$$[sizeof(real) * k_number_of_campaign_difficulty_levels];
			byte $$$$$[sizeof(real) * k_number_of_campaign_difficulty_levels];

			// placement
			// 
			// difficulty-affecting values for enemy placement
			real major_upgrade_normal[k_number_of_campaign_difficulty_levels]; // fraction of actors upgraded to their major variant.
			real major_upgrade_few[k_number_of_campaign_difficulty_levels];    // fraction of actors upgraded to their major variant when mix = normal.
			real major_upgrade_many[k_number_of_campaign_difficulty_levels];   // fraction of actors upgraded to their major variant when mix = many.

			// vehicles
			// 
			// difficulty-affecting values for vehicle driving/combat
			real player_vehicle_ram_chance[k_number_of_campaign_difficulty_levels]; // Chance of deciding to ram the player in a vehicle
			byte $$$$$$[sizeof(real) * k_number_of_campaign_difficulty_levels];
			byte $$$$$$$[sizeof(real) * k_number_of_campaign_difficulty_levels];
			byte $$$$$$$$[sizeof(real) * k_number_of_campaign_difficulty_levels];
		};
		real value[NUMBER_OF_GAME_DIFFICULTY_VALUES][k_number_of_campaign_difficulty_levels];
	};

	byte Q[0x54];
};
static_assert(sizeof(s_game_globals_difficulty_information) == 0x284);

struct s_game_globals
{
	byte data[0x120];
	c_typed_tag_block<s_game_globals_difficulty_information> difficulty;
	byte data2[0x84];
	c_typed_tag_reference<MULTIPLAYER_GLOBALS_TAG> multiplayer_globals;
	c_typed_tag_reference<SURVIVAL_MODE_GLOBALS_TAG> survival_globals;
	c_typed_tag_reference<HF2P_GLOBALS_TAG> armor_globals;
	byte data3[0x434];
};
static_assert(sizeof(s_game_globals) == 0x614);
static_assert(0x120 == OFFSETOF(s_game_globals, difficulty));
static_assert(0x1B0 == OFFSETOF(s_game_globals, multiplayer_globals));
static_assert(0x1D0 == OFFSETOF(s_game_globals, armor_globals));

real game_difficulty_get_value_by_difficulty(short value_type, e_campaign_difficulty_level difficulty_level);
real __fastcall game_difficulty_get_team_value(short value_type, e_game_team team);
s_game_globals_difficulty_information* game_globals_get_difficulty_information(s_game_globals* game_globals);

extern s_game_globals*& global_game_globals;