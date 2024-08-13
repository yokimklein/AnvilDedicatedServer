#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <tag_files\tag_groups.h>
#include <camera\camera_globals.h>
#include <game\materials.h>

enum e_damage_reporting_type
{
	_damage_reporting_type_guardians_unknown,
	_damage_reporting_type_guardians,
	_damage_reporting_type_falling_damage,
	_damage_reporting_type_generic_collision,
	_damage_reporting_type_armor_lock_crush,
	_damage_reporting_type_generic_melee,
	_damage_reporting_type_generic_explosion,
	_damage_reporting_type_magnum,
	_damage_reporting_type_plasma_pistol,
	_damage_reporting_type_needler,
	_damage_reporting_type_mauler,
	_damage_reporting_type_smg,
	_damage_reporting_type_plasma_rifle,
	_damage_reporting_type_battle_rifle,
	_damage_reporting_type_carbine,
	_damage_reporting_type_shotgun,
	_damage_reporting_type_sniper_rifle,
	_damage_reporting_type_beam_rifle,
	_damage_reporting_type_assault_rifle,
	_damage_reporting_type_spiker,
	_damage_reporting_type_fuel_rod_cannon,
	_damage_reporting_type_missile_pod,
	_damage_reporting_type_rocket_launcher,
	_damage_reporting_type_spartan_laser,
	_damage_reporting_type_brute_shot,
	_damage_reporting_type_flamethrower,
	_damage_reporting_type_sentinel_gun,
	_damage_reporting_type_energy_sword,
	_damage_reporting_type_gravity_hammer,
	_damage_reporting_type_frag_grenade,
	_damage_reporting_type_plasma_grenade,
	_damage_reporting_type_spike_grenade,
	_damage_reporting_type_firebomb_grenade,
	_damage_reporting_type_flag,
	_damage_reporting_type_bomb,
	_damage_reporting_type_bomb_explosion,
	_damage_reporting_type_ball,
	_damage_reporting_type_machinegun_turret,
	_damage_reporting_type_plasma_cannon,
	_damage_reporting_type_plasma_mortar,
	_damage_reporting_type_plasma_turret,
	_damage_reporting_type_shade_turret,
	_damage_reporting_type_banshee,
	_damage_reporting_type_ghost,
	_damage_reporting_type_mongoose,
	_damage_reporting_type_scorpion,
	_damage_reporting_type_scorpion_gunner,
	_damage_reporting_type_spectre,
	_damage_reporting_type_spectre_gunner,
	_damage_reporting_type_warthog,
	_damage_reporting_type_warthog_gunner,
	_damage_reporting_type_warthog_gauss_turret,
	_damage_reporting_type_wraith,
	_damage_reporting_type_wraith_gunner,
	_damage_reporting_type_tank,
	_damage_reporting_type_chopper,
	_damage_reporting_type_hornet,
	_damage_reporting_type_mantis,
	_damage_reporting_type_prowler,
	_damage_reporting_type_sentinel_beam,
	_damage_reporting_type_sentinel_rpg,
	_damage_reporting_type_teleporter,
	_damage_reporting_type_tripmine,
	_damage_reporting_type_dmr,

	k_damage_reporting_type_count
};

enum e_damage_effect_flags
{
	_damage_effect_flag_dont_scale_damage_by_distance,
	_damage_effect_flag_area_damage_players_only,
	// affects model targets flag ODST or Reach+?

	k_damage_effect_flags_count
};

enum e_damage_flags
{
	_damage_flag_does_not_hurt_owner,
	_damage_flag_can_cause_headshots,
	_damage_flag_pings_resistant_units,
	_damage_flag_does_not_hurt_friends,
	_damage_flag_does_not_ping_units,
	_damage_flag_detonates_explosives,
	_damage_flag_only_hurts_shields,
	_damage_flag_causes_flaming_death,
	_damage_flag_damage_indicators_always_point_down,
	_damage_flag_skips_shields,
	_damage_flag_only_hurts_one_infection_form,
	_damage_flag_transfer_dmg_always_uses_min,
	_damage_flag_infection_form_pop,
	_damage_flag_ignore_seat_scale_for_dir_dmg,
	_damage_flag_forces_hard_ping_if_body_dmg,
	_damage_flag_does_not_hurt_players,
	_damage_flag_does_not_overcombine,
	_damage_flag_enables_special_death,
	_damage_flag_cannot_cause_betrayals,
	_damage_flag_uses_old_emp_behaviour,
	_damage_flag_ignores_damage_resistance,
	_damage_flag_force_s_kill_on_death,
	_damage_flag_cause_magic_deceleration,
	_damage_flag_inhibits_melee_attacks,
	_damage_flag_concussive_blast,

	k_damage_flags_count
};

struct s_damage_reporting_info
{
	c_enum<e_damage_reporting_type, short, _damage_reporting_type_guardians_unknown, k_damage_reporting_type_count> type;
	byte modifier;
	byte_flags recycling_flag;
};
static_assert(sizeof(s_damage_reporting_info) == 0x4);

struct s_damage_owner
{
	datum_index player_index;
	datum_index object_index;
	byte team;
};
static_assert(sizeof(s_damage_owner) == 0xC);

struct damage_definition
{
	short side_effect;
	short category;
	c_flags<e_damage_flags, long, k_damage_flags_count> flags;
	real aoe_core_radius;
	real damage_lower_bound;
	real_bounds damage_upper_bound;
	angle dmg_inner_cone_angle;
	angle dmg_outer_cone_angle;
	real active_camouflage_damage;
	real stun;
	real maximum_stun;
	real stun_time;
	real intantaneous_acceleration;
	real rider_direct_damage_scale;
	real rider_maximum_transfer_damage_scale;
	real rider_minimum_transfer_damage_scale;
	string_id general_damage;
	string_id specific_damage;
	string_id custom_response_label;
	real ai_stun_radius;
	real_bounds ai_stun_bounds;
	real shake_radius;
	real emp_radius;
	real aoe_spike_radius;
	real aoe_spike_damage_bump;
	real shield_render_effects_radius;
};
static_assert(sizeof(damage_definition) == 0x6C);

struct damage_effect_player_response
{

};
//static_assert(sizeof(damage_effect_player_response) == 0x0);

struct camera_impulse
{
	real impulse_duration;
	c_enum<e_global_transition_function, short, _global_transition_function_linear, k_global_transition_function_count> fade_function;
	short : 16;
	angle rotation;
	real pushback; // world units
	real_bounds jitter;
};
static_assert(sizeof(camera_impulse) == 0x18);

struct camera_shake
{
	real duration;
	c_enum<e_global_transition_function, short, _global_transition_function_linear, k_global_transition_function_count> falloff_function;
	short : 16;
	real random_translation;
	angle random_rotation;
	c_enum<e_global_periodic_function, short, _global_periodic_function_one, k_global_periodic_function_count> wobble_function;
	real wobble_function_period;
	real wobble_weight;
};
static_assert(sizeof(camera_shake) == 0x1C);

struct breaking_effect
{
	real forward_velocity; // world units per second
	real forward_radius; // world units
	real forward_exponent;
	real outward_velocity; // world units per second
	real outward_radius; // world units
	real outward_exponent;
};
static_assert(sizeof(breaking_effect) == 0x18);

struct s_damage_effect_definition
{
	real_bounds radius;
	real cutoff_scale;
	c_flags<e_damage_effect_flags, long, k_damage_effect_flags_count> effect_flags;
	damage_definition damage;
	c_typed_tag_block<damage_effect_player_response> player_responses;
	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG> damage_response;
	camera_impulse temporary_camera_impulse;
	camera_shake camera_shaking;
	c_typed_tag_reference<SOUND_TAG> sound;
	breaking_effect breaking_effect;
};
static_assert(sizeof(s_damage_effect_definition) == 0xF4);

struct c_aoe_damage_batchifier;
struct s_damage_data
{
	long damage_effect_definition_index;
	dword_flags flags;
	s_damage_owner damage_owner;

	long __unknown14;
	long __unknown18;

	long damage_unique_identifier;
	s_location location;

	byte __data22[2];

	real_point3d origin;
	real_point3d center;
	real_vector3d attacker_direction;

	real_vector3d __vector48;

	real damage_amount_scale;
	real shake_scale;
	real damage_amount;
	real damage_aoe_size;

	real __unknown64;
	real __unknown68;
	bool __unknown6C;
	bool __unknown6D;
	byte __data6E[0x2];
	real_vector3d __vector70;
	long __unknown7C;

	real vitality;
	c_global_material_type material_type;

	short __unknown86;
	byte __data88[0x4];

	s_damage_reporting_info damage_reporting_info;
	c_aoe_damage_batchifier* aoe_damage_batchifier;
	long damage_material_index; // model_definition->materials[damage_material_index]
};
static_assert(sizeof(s_damage_data) == 0x98);