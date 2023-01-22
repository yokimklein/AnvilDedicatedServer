#pragma once
#include "..\cseries\cseries.h"

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

struct s_damage_reporting_info
{
	c_enum<e_damage_reporting_type, short, k_damage_reporting_type_count> type;
	char variant; // weapon variant
	char modifier;
};
static_assert(sizeof(s_damage_reporting_info) == 0x4);

struct s_damage_owner
{
	long player_index;
	long object_index;
	byte team;
};
static_assert(sizeof(s_damage_owner) == 0xC);