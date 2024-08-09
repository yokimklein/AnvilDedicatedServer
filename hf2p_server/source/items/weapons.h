#pragma once
#include <items\items.h>
#include <objects\target_tracking.h>
#include <animations\animation_data.h>

enum e_weapon_barrel_state
{
	_weapon_barrel_state_idle,
	_weapon_barrel_state_firing,

	k_weapon_barrel_state_count
};

struct s_weapon_barrel
{
	uchar flags;
	uchar state;
	ushort : 16;
	ushort firing_flags;
	ushort firing_time;
	long : 32;
	long : 32;
	real firing_rate;
	long : 32;
	real firing_effect;
	real error;
	real recoil;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};
static_assert(sizeof(s_weapon_barrel) == 0x34);

struct s_weapon_trigger
{
	char field_0;
	char field_1;
	char field_2;
	char field_3;
	short charge_up_timer;
	ushort flags;
	long charing_effect_index;
};
static_assert(sizeof(s_weapon_trigger) == 0xC);

struct s_weapon_magazine
{
	short state;
	short reload_cooldown;
	short field_4;
	short rounds_unloaded;
	short field_8;
	short rounds_loaded;
	short field_C;
	short field_E;
	short field_10;
	short firing_cooldown;
	short field_14;
	short field_16;
	short field_18;
};
static_assert(sizeof(s_weapon_magazine) == 0x1A);

struct s_weapon_first_person_emulation
{
	long animation_graph_definition_index;
	c_animation_id animation_id;
	long authored_time;
	long animation_name;
	long field_10;
	long sound_impulse_tracker_index;
	long unknown_animation_name;
};
static_assert(sizeof(s_weapon_first_person_emulation) == 0x1C);

struct _weapon_datum
{
	ushort flags;
	char control_flags;
	char primary_trigger;
	char last_primary_trigger;
	char last_hill_or_valley;
	char primary_trigger_diration;
	char primary_trigger_down_ticks;
	char primary_trigger_firing_ticks;
	char barrel_spin;
	char delay_authorative_state_timer;
	char field_197;
	short state;
	short state_timer;
	short weapon_disabled_by_reload_timer;
	short multiplayer_weapon_identifier;
	short simulation_weapon_identifier;
	byte turn_on_timer;
	byte ready_for_use_timer;
	real heat;
	real age;
	real overcharged;
	real illumination_scale;
	real current_power;
	real desired_power;
	c_target_tracking_system target_tracking;
	short recoil_angular_velocity;
	short recoil_recovery_time;
	short shorts_until_dometion;
	short alternate_shots_loaded;
	s_weapon_barrel barrels[2];
	s_weapon_trigger triggers[2];
	s_weapon_magazine magazines[2];
	long overheated_effect_index;
	long game_time_last_fired;
	s_weapon_first_person_emulation first_person_emulation;
};
static_assert(sizeof(_weapon_datum) == 0x150);

struct weapon_datum
{
	long definition_index;
	_object_datum object;
	_item_datum item;
	_weapon_datum weapon;
};
static_assert(sizeof(weapon_datum) == 0x2E0);

datum_index weapon_get_owner_unit_index(datum_index weapon_index);
void __fastcall weapon_delay_predicted_state(datum_index weapon_index);
long __fastcall weapon_get_owner_unit_inventory_index(datum_index weapon_index);