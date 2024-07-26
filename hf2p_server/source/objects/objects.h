#pragma once
#include <memory\data.h>
#include <cseries\cseries.h>
#include <objects\object_types.h>
#include <scenario\scenario_definitions.h>
#include <objects\damage.h>

enum e_object_header_flags
{
	_object_header_active_bit = 0,
	_object_header_awake_bit = 1,
	_object_header_requires_motion_bit = 2,
	_object_header_post_update_bit = 3,
	_object_header_being_deleted_bit = 4,
	_object_header_do_not_update_bit = 5,
	_object_header_connected_to_map_bit = 6,
	_object_header_child_bit = 7,

	k_number_of_object_header_flags
};

enum e_object_data_flags
{
	_object_hidden_bit,
	_object_always_active_bit,
	_object_being_created_bit,
	_object_unknown3_bit, // _object_is_submerged_bit, // when set, the object can't be a simulation multiplayer item
	_object_has_attached_lights_bit,
	_object_has_attached_looping_sounds_bit,
	_object_has_unattached_lights_bit,
	_object_in_limbo_bit,
	_object_connected_to_map_bit,
	_object_uses_collidable_list_bit,
	_object_mirrored_bit,
	_object_render_model_has_instances_bit,
	_object_static_lighting_recompute_bit,
	_object_static_lighting_raycast_sideways_bit,
	_object_render_time_node_matrices_complete_bit,
	_object_clip_plane_bit,
	_object_shadowless_bit,
	_object_deleted_when_deactivated_bit,
	_object_outside_of_map_bit,
	_object_cinematic_lod_bit,
	_object_cinematic_collision_bit,
	_object_cinematic_visibility_bit,
	_object_static_pathfinding_bit,
	_object_dynamic_pathfinding_disabled_bit,
	_object_uses_cinematic_lighting_bit,
	_object_has_override_bit,
	_object_created_with_parent_bit,
	_object_reconnect_to_map_bit,
	_object_ever_referenced_by_hs_bit,
	_object_orientations_frozen_bit,
	_object_render_only_orientations_needed_bit,
	_object_is_prt_and_lightmapped_bit,

	k_object_data_flags
};

enum e_object_recycling_flags
{
	_object_recycling_unknown0,
	_object_recycling_candidate,
	_object_recycling_unknown2,
	_object_recycling_unknown3,
	_object_recycling_unknown4,

	k_number_of_object_recycling_flags
};

// verified for ms23
enum e_object_simulation_flags
{
	_object_simulation_requires_interpolation_bit,
	_object_simulation_is_multiplayer_cinematic_object,
	//_object_simulation_has_used_death_grenade_bit = 2, // assumed - unused? string ref in h3debug
	//_object_simulation_is_multiplayer_item = 2, // non original name, not used after h3debug?
	_object_simulation_candy_monitored_object = 2, // non original name

	k_number_of_object_simulation_flags
};

enum e_object_physics_flags
{
	_object_is_early_mover_bit = 0,
	_object_is_early_mover_child_bit = 4,
	_object_connected_to_physics_bit = 7,
	_object_physics_unknown_9_bit = 9,
	_object_has_proxy_bit = 14,

	k_number_of_object_physics_flags
};

class c_object_identifier
{
public:
	e_object_type get_type();

	datum_index unique_id;
	short origin_bsp_index;
	c_enum<e_object_type, byte, k_object_type_count> type;
	byte source;
};
static_assert(sizeof(c_object_identifier) == 0x8);

struct s_object_header_block_reference
{
	short size;
	short offset;
};
static_assert(sizeof(s_object_header_block_reference) == 0x4);

struct s_object_damage_section
{
	long unknown1;
	long unknown2;
	long unknown3;
};
static_assert(sizeof(s_object_damage_section) == 0xC);

struct s_object_data
{
	long definition_index;
	c_flags<e_object_data_flags, long, k_object_data_flags> flags;
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
	real bounding_sphere_radius;
	real_point3d collision_sphere_center;
	real collision_sphere_radius;
	real_point3d attached_bounds_sphere_center;
	real attached_bounds_sphere_radius;
	long first_cluster_reference_index;
	real_point3d relative_position;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d transitional_velocity;
	real_vector3d angular_velocity;
	real scale;
	c_object_identifier object_identifier;
	short name_index;
	char bsp_placement_policy;
	char keyframed_object_collision_damage_ticks;
	datum_index havok_component_index;
	long local_physics_space_object_index;
	long last_motion_time;
	c_flags<e_object_physics_flags, long, k_number_of_object_physics_flags> physics_flags;
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
	real maximum_body_vitality;
	real maximum_shield_vitality;
	real shield_charge_delay;
	real body_vitality;
	real shield_vitality;
	real current_body_damage;
	real current_shield_damage;
	real recent_body_damage;
	real recent_shield_damage;
	real shield_impact_current_body_damage;
	real shield_impact_current_shield_damage;
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
	long sync_action_name; // 0x13C
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
	long air_probe_index;
	long air_probe_index2;
	long unknown; // TODO: verify where this new field goes!
};
static_assert(sizeof(s_object_data) == 0x17C);
static_assert(0x13C == OFFSETOF(s_object_data, sync_action_name));
static_assert(0x11C == OFFSETOF(s_object_data, shield_stun_ticks));

struct s_object_header : s_datum_header
{
	c_flags<e_object_header_flags, byte, k_number_of_object_header_flags> flags;
	c_enum<e_object_type, byte, k_object_type_count> type;
	short cluster_reference;
	word data_size;
	long pool_handle;
	s_object_data* data;
};
static_assert(sizeof(s_object_header) == 0x10);

s_object_data* __fastcall object_try_and_get_and_verify_type(datum_index object_index, dword object_type_mask);
s_object_data* object_get(datum_index object_index);
e_object_type object_get_type(datum_index object_index);
bool object_is_multiplayer_cinematic_object(datum_index object_index);
datum_index object_get_ultimate_parent(datum_index object_index);
void object_attach_gamestate_entity(datum_index object_index, datum_index gamestate_index);
void object_detach_gamestate_entity(datum_index object_index, datum_index gamestate_index);
void __cdecl object_set_velocities_internal(datum_index object_index, real_vector3d const* transitional_velocity, real_vector3d const* angular_velocity, bool skip_update);
void __fastcall object_set_at_rest(datum_index object_index, bool force_activate);
const char* object_describe(datum_index object_index);
void __fastcall object_set_damage_owner(datum_index object_index, s_damage_owner* damage_owner, bool skip_update);
void __fastcall object_wake(datum_index object_index);
void __fastcall object_set_requires_motion(datum_index object_index);
bool __fastcall object_needs_rigid_body_update(datum_index object_index);
void __fastcall attachments_update(datum_index object_index);
void __fastcall object_compute_node_matrices(datum_index object_index);
datum_index __fastcall object_new(s_object_placement_data* placement_data);
void __fastcall object_set_garbage(datum_index object_index, bool unknown_bool, long collection_ticks);
bool __fastcall object_set_position_internal(datum_index object_index, real_point3d* desired_position, real_vector3d* desired_forward, real_vector3d* desired_up, s_location const* location, bool compute_node_matrices, bool set_havok_object_position, bool in_editor, bool disconnected);