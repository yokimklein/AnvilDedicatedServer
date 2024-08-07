#pragma once
#include <cseries\cseries.h>
#include <simulation\game_interface\simulation_game_entities.h>

enum e_simulation_object_update_flag
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

struct s_simulation_object_state_data
{
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	real scale;
	real_vector3d transitional_velocity;
	real_vector3d angular_velocity;
	real body_vitality;
	bool body_stunned;
	real shield_vitality;
	bool shield_stunned;
	bool shield_charging;
	bool shield_depleted;
	bool dead;
	short owner_team_index;
	byte region_count;
	byte region_state[16];
	byte constraint_count;
	short destroyed_constraints;
	short loosened_constraints;
	long replication_entity_index;
	long gamestate_index;
	long node;
	bool initial_attachment;
	real_point3d attachment_position;
	real_vector3d attachment_up;
	real_vector3d attachment_forward;
	word_flags multiplayer_properties_data_flags;
	short map_variant_index;
};
static_assert(sizeof(s_simulation_object_state_data) == 0xA0);

struct c_simulation_object_index_state_data
{
	long m_entity_index;
	long m_simulation_object_glue_index;
};
static_assert(sizeof(c_simulation_object_index_state_data) == 0x8);

// CAREFUL USING THIS IN HOOKS - constructor is called by default and will immediately use a register!
class c_simulation_object_update_flags
{
public:
	c_simulation_object_update_flags()
	{
		m_flags.clear();
	}

	template<typename t_update_type>
	void set_flag(datum_index object_index, t_update_type update_flag)
	{
		long flag = *reinterpret_cast<long*>(&update_flag);
		m_flags.set(flag, true);
	};
	void set_raw(ulong64 raw_bits)
	{
		m_flags.set_unsafe(raw_bits);
	}

	c_flags<long, ulong64, 64> m_flags;
};

void simulation_action_object_update_internal(datum_index object_index, c_simulation_object_update_flags update_flags);

template <typename t_update_type>
void __fastcall simulation_action_object_update(datum_index object_index, t_update_type update_type)
{
	c_simulation_object_update_flags update_flags;
	update_flags.set_flag(object_index, update_type);
	simulation_action_object_update_internal(object_index, update_flags);
}

void __cdecl simulation_action_object_create(datum_index object_index);
void simulation_action_object_create_build_entity_types(datum_index object_index, datum_index last_object_index, long maximum_entity_count, long* out_entity_count, e_simulation_entity_type* entity_types, long* object_indices);
void simulation_action_object_delete(datum_index object_index);
long simulation_object_get_authoritative_entity(datum_index object_index);
long simulation_object_get_entity_internal(datum_index object_index, bool safe);