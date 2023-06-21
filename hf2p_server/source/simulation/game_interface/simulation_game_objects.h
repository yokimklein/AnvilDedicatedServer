#pragma once
#include <simulation\game_interface\simulation_game_entities.h>
#include <cseries\cseries.h>

enum e_simulation_object_update_flag : long
{
	_simulation_object_update_dead,
	_simulation_object_update_position,
	_simulation_object_update_forward_and_up,
	_simulation_object_update_scale,
	_simulation_object_update_translational_velocity,
	_simulation_object_update_angular_velocity,
	_simulation_object_update_body_vitality, // vitality & stun ticks
	_simulation_object_update_shield_vitality,
	_simulation_object_update_region_state,
	_simulation_object_update_constraint_state,
	_simulation_object_update_owner_team_index,
	_simulation_object_update_parent_state,
	_simulation_object_update_multiplayer_properties,
	_simulation_object_update_map_variant_index,

	k_simulation_object_update_flag_count
};

enum e_simulation_unit_update_flag : long
{
	_simulation_unit_update_control = k_simulation_object_update_flag_count,
	_simulation_unit_update_parent_vehicle,
	_simulation_unit_update_desired_aiming_vector,
	_simulation_unit_update_desired_weapon_set,
	_simulation_unit_update_weapon1_type,
	_simulation_unit_update_weapon2_type,
	_simulation_unit_update_weapon3_type,
	_simulation_unit_update_weapon4_type,
	_simulation_unit_update_weapon1_state,
	_simulation_unit_update_weapon2_state,
	_simulation_unit_update_weapon3_state,
	_simulation_unit_update_weapon4_state,
	_simulation_unit_update_grenade_counts,
	_simulation_unit_update_active_camo,
	_simulation_unit_update_equipment,
	_simulation_unit_update_equipment_charges,
	_simulation_unit_update_consumable_energy,
	_simulation_unit_update_control_context,
	_simulation_unit_update_assassination_data,
	// removed from HO
	//_simulation_unit_update_hud_info,
	//_simulation_unit_update_map_editor_flags,

	k_simulation_unit_update_flag_count
};

enum e_simulation_item_update_flag : long
{
	_simulation_item_update_set_at_rest = k_simulation_object_update_flag_count,
	// equipment only
	_simulation_item_update_equipment_begin_animation_state,
	_simulation_item_update_equipment_creation_time,
	_simulation_item_update_equipment_owner,

	k_simulation_item_update_flag_count
};

enum e_simulation_projectile_update_flag : long
{
	_simulation_projectile_update_set_at_rest = k_simulation_object_update_flag_count,
	// surprisingly nothing was added for projectile latching

	k_simulation_projectile_update_flag_count
};

enum e_simulation_weapon_update_flag : long
{
	_simulation_weapon_update_weapon_flags = k_simulation_item_update_flag_count,
	_simulation_weapon_update_multiplayer_weapon_registration,
	_simulation_weapon_update_ammo,

	k_simulation_weapon_update_flag_count
};

enum e_simulation_generic_update_flag : long
{
	_simulation_generic_update_rigid_body = k_simulation_object_update_flag_count,

	k_simulation_generic_update_flag_count
};

enum e_simulation_device_update_flag : long
{
	_simulation_device_update_position = k_simulation_object_update_flag_count,
	_simulation_device_update_15, // TODO
	_simulation_device_update_power,
	_simulation_device_update_17, // power group related

	k_simulation_device_update_flag_count
};

enum e_simulation_vehicle_update_flag : long
{
	_simulation_vehicle_update_auto_turret = k_simulation_object_update_flag_count, // auto turret initialisation?
	_simulation_vehicle_update_auto_turret_tracking, // auto turret tracking?
	_simulation_vehicle_update_seat_power,
	_simulation_vehicle_update_active_camo,

	k_simulation_vehicle_update_flag_count
};

void __cdecl simulation_action_object_create(datum_index object_index);
void simulation_action_object_create_build_entity_types(datum_index object_index, datum_index last_object_index, long maximum_entity_count, long* out_entity_count, e_simulation_entity_type* entity_types, long* object_indices);
void simulation_action_object_delete(datum_index object_index);


class c_simulation_object_update_flags
{
public:
	template<typename t_update_type>
	void set_flag(datum_index object_index, t_update_type update_flag)
	{
		long flag = *reinterpret_cast<long*>(&update_flag);
		m_flags.set(flag, true);
	};
	void set_raw(ulong64 raw_bits)
	{
		m_flags.set_raw_bits(raw_bits);
	}

	c_flags<long, ulong64, 64> m_flags;
};

void simulation_action_object_update_internal(datum_index object_index, c_simulation_object_update_flags update_flags);

template <typename t_update_type>
void __fastcall simulation_action_object_update(datum_index object_index, t_update_type update_type)
{
	c_simulation_object_update_flags update_flags = c_simulation_object_update_flags();
	update_flags.set_flag(object_index, update_type);
	simulation_action_object_update_internal(object_index, update_flags);
}