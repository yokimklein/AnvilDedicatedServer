#pragma once
#include "..\memory\data.h"
#include "..\cseries\cseries.h"
#include "object_types.h"
#include "..\scenario\scenario_definitions.h"
#include "damage.h"

enum e_object_header_flags
{
	_object_header_being_deleted_bit = 4,

	k_number_of_object_header_flags
};

enum e_object_flags
{
	_object_unknown0_bit,
	_object_unknown1_bit,
	_object_unknown2_bit,
	_object_unknown3_bit,
	_object_unknown4_bit,
	_object_unknown5_bit,
	_object_unknown6_bit,
	_object_unknown7_bit,
	_object_unknown8_bit,
	_object_unknown9_bit,
	_object_unknown10_bit,
	_object_unknown11_bit,
	_object_unknown12_bit,
	_object_unknown13_bit,
	_object_unknown14_bit,
	_object_unknown15_bit,
	_object_unknown16_bit,
	_object_unknown17_bit,
	_object_unknown18_bit,
	_object_unknown19_bit,
	_object_unknown20_bit,
	_object_unknown21_bit,
	_object_unknown22_bit,
	_object_unknown23_bit,
	_object_unknown24_bit,
	_object_unknown25_bit,
	_object_unknown26_bit,
	_object_unknown27_bit,
	_object_unknown28_bit,
	_object_unknown29_bit,
	_object_unknown30_bit,
	_object_unknown31_bit,

	k_number_of_object_flags
};

enum e_object_recycling_flags
{
	_object_recycling_bit0,
	_object_recycling_bit1,

	k_number_of_object_recycling_flags
};

enum e_object_simulation_flags
{
	_object_simulation_bit0,
	_object_simulation_is_cinematic_object,

	k_number_of_object_simulation_flags
};

class c_object_identifier
{
public:
	e_object_data_flags get_type();

	datum_index unique_id;
	short origin_bsp_index;
	c_enum<e_object_data_flags, byte, k_number_of_object_data_flags> type;
	byte source;
};
static_assert(sizeof(c_object_identifier) == 0x8);

struct s_object_header_block_reference
{
	short size;
	short offset;
};
static_assert(sizeof(s_object_header_block_reference) == 0x4);

struct s_object_data
{
	long definition_index;
	c_flags<e_object_flags, long, k_number_of_object_flags> object_flags;
	ulong extra_object_state;
	datum_index next_object_index;
	datum_index first_child_object_index;
	datum_index parent_object_index;
	uchar parent_node_index;
	uchar inhibited_flags;
	short scenario_datum_index;
	short map_variant_index;
	s_scenario_location location;
	real_point3d bounding_sphere_center;
	float bounding_sphere_radius;
	real_point3d collision_sphere_center;
	float collision_sphere_radius;
	real_point3d attached_bounds_sphere_center;
	float attached_bounds_sphere_radius;
	long first_cluster_reference_index;
	real_point3d relative_position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d transitional_velocity;
	real_vector3d angular_velocity;
	float scale;
	c_object_identifier object_identifier;
	short name_index;
	char bsp_placement_policy;
	char keyframed_object_collision_damage_ticks;
	long havok_component_index;
	long local_physics_space_object_index;
	long last_motion_time;
	ulong physics_flags;
	uchar physics_deactivation_count;
	uchar physically_attached_node;
	uchar phantom_power_scale;
	uchar phantom_speed_scale;
	short in_water_ticks;
	char environment_interpenetration_ticks;
	uchar brittle_collision_damage_timer;
	short scale_intepolation_ticks;
	char variant_index;
	uchar ai_flags;
	ulong melee_damage_unique_identifier;
	s_damage_owner damage_owner;
	short bsp2E4_datum_index;
	char created_at_rest;
	uchar clusters_touched_on_connection;
	datum_index gamestate_index; // object.gamestate_index
	short simulation_unknown_team_index;
	c_flags<e_object_simulation_flags, byte, k_number_of_object_simulation_flags> simulation_flags;
	char child_variant_index;
	long unknown_simulation_ticks;
	real_vector3d unknown_simulation_position;
	long first_widget_index;
	short destroyed_constraints;
	short loosened_constraints;
	float maximum_body_vitality;
	float maximum_shield_vitality;
	float shield_charge_delay;
	float body_vitality;
	float shield_vitality;
	float current_body_damage;
	float current_shield_damage;
	float recent_body_damage;
	float recent_shield_damage;
	float shield_impact_current_body_damage;
	float shield_impact_current_shield_damage;
	short shield_stun_ticks;
	short body_stun_ticks;
	ulong damage_flags;
	char damaged_explosion_timer;
	char body_damage_delay_ticks;
	char shield_impact_decay_timer;
	c_flags<e_object_recycling_flags, byte, k_number_of_object_recycling_flags> recycling_flags;
	long next_recycling_object_index;
	long recycling_time;
	long parent_recycling_group;
	long next_recycling_group_member;
	long next_sync_action_participant_index;
	long sync_action_name;
	long sync_animation_id;
	long ai_sync_action_arranger_index;
	short render_flags;
	s_object_header_block_reference original_node_orientations;
	s_object_header_block_reference node_orientations;
	s_object_header_block_reference node_matrices;
	s_object_header_block_reference region_information;
	s_object_header_block_reference attachments;
	s_object_header_block_reference damage_sections;
	s_object_header_block_reference change_colors;
	s_object_header_block_reference animation;
	s_object_header_block_reference multiplayer;
	short first_extension_offset;
	long air_probe_index;
	long air_probe_index2;
};
static_assert(sizeof(s_object_data) == 0x178);

struct s_object_header : s_datum_header
{
	c_flags<e_object_header_flags, byte, k_number_of_object_header_flags> flags;
	c_enum<e_object_data_flags, byte, k_number_of_object_data_flags> type;
	short cluster;
	word data_size;
	long pool_handle;
	s_object_data* data;
};
static_assert(sizeof(s_object_header) == 0x10);

s_object_data* object_get(datum_index object_index);
bool object_is_multiplayer_cinematic_object(datum_index object_index);
datum_index object_get_ultimate_parent(datum_index object_index);
void object_attach_gamestate_entity(datum_index object_index, datum_index gamestate_index);
void object_detach_gamestate_entity(datum_index object_index, datum_index gamestate_index);