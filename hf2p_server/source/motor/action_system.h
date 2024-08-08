#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

enum e_action
{
	_action_none = NONE,
	_action_death_ping,
	_action_custom_animation,
	_action_stop_custom_animation,
	_action_sync_container,
	_action_assassinate,
	_action_get_infected,
	_action_hostile_morph,
	_action_resurrect,
	_action_hard_ping,
	_action_ping,
	_action_morph,
	_action_morph_arrival,
	_action_infect,
	_action_posture,
	_action_posture_exit,
	_action_vehicle_exit,
	_action_vehicle_exit_and_detach,
	_action_vehicle_ejection,
	_action_vehicle_board,
	_action_vehicle_enter,
	_action_open,
	_action_close,
	_action_vehicle_exit_immediate,
	_action_vehicle_flip,
	_action_climb_attach,
	_action_climb_detach,
	_action_wall_lean,
	_action_wall_lean_open,
	_action_token,
	_action_takeoff,
	_action_perch,
	_action_perch_jump,
	_action_land,
	_action_jump,
	_action_leap_anticipation,
	_action_leap,
	_action_leap_recovery,
	_action_emit_ai,
	_action_berserk,
	_action_flurry,
	_action_deploy_turret,
	_action_smash,
	_action_vault,
	_action_hoist,
	_action_ai_impulse,
	_action_melee_ai,
	_action_weapon_pickup,
	_action_weapon_trade,
	_action_primary_weapon_drop,
	_action_secondary_weapon_drop,
	_action_surge,
	_action_phase,
	_action_stow,
	_action_draw,
	_action_infection_spew,
	_action_kamikaze,
	_action_primary_weapon_switch,
	_action_secondary_weapon_switch,
	_action_grenade,
	_action_equipment,
	_action_equipment_animation,
	_action_equipment_pickup,
	_action_equipment_drop,
	_action_melee_player,
	_action_primary_weapon_primary_reload,
	_action_primary_weapon_secondary_reload,
	_action_secondary_weapon_primary_reload,
	_action_secondary_weapon_secondary_reload,
	_action_soft_ping,
	_action_primary_weapon_primary_recoil,
	_action_primary_weapon_secondary_recoil,
	_action_primary_weapon_primary_chamber,
	_action_primary_weapon_secondary_chamber,
	_action_primary_weapon_primary_charged,
	_action_primary_weapon_secondary_charged,
	_action_secondary_weapon_primary_recoil,
	_action_secondary_weapon_secondary_recoil,
	_action_secondary_weapon_primary_chamber,
	_action_secondary_weapon_secondary_chamber,
	_action_secondary_weapon_primary_charged,
	_action_secondary_weapon_secondary_charged,
	_action_primary_weapon_overheat_enter,
	_action_primary_weapon_overheat_loop,
	_action_primary_weapon_overheat_exit,
	_action_secondary_weapon_overheat_enter,
	_action_secondary_weapon_overheat_loop,
	_action_secondary_weapon_overheat_exit,
	_action_player_taunt,

	k_number_of_actions
};

struct s_unit_assassinate_action_request
{
	datum_index victim_unit_index;
	string_id sync_action_name;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
};
static_assert(sizeof(s_unit_assassinate_action_request) == 0x2C);

struct s_action_request
{
	c_enum<e_action, long, _action_none, k_number_of_actions> action_type;
	union
	{
		s_unit_assassinate_action_request assassination;
	};
};