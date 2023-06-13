#pragma once
#include <tag_files\tag_field.h>
#include <tag_files\tag_block.h>
#include <models\damage_info_definitions.h>
#include <math\real_math.h>

constexpr long k_maximum_number_of_model_regions = 16;
constexpr long k_maximum_number_of_model_region_permutations = 12;

enum e_model_lod
{
	_model_lod_super_low,
	_model_lod_low,
	_model_lod_medium,
	_model_lod_high,
	_model_lod_super_high,

	k_number_of_model_lods
};

enum e_model_region_sort_order
{
	_model_region_no_sorting,
	_model_region_sort_closest,
	_model_region_sort_much_more_closer,
	_model_region_sort_much_closer,
	_model_region_sort_closer,
	_model_region_sort_close,
	_model_region_sort_same_as_model,
	_model_region_sort_far,
	_model_region_sort_farther,
	_model_region_sort_much_farther,
	_model_region_sort_much_more_farther,
	_model_region_sort_farthest,

	k_number_of_model_region_sort_orders
};

enum e_model_ai_character
{
	_model_ai_character_none,
	_model_ai_character_rookie,
	_model_ai_character_buck,
	_model_ai_character_dare,
	_model_ai_character_dutch,
	_model_ai_character_johnson,
	_model_ai_character_mickey,
	_model_ai_character_romeo,

	k_number_of_model_ai_characters
};

enum e_model_permutation_flags
{
	_model_permutation_copy_states_to_all_permutations_bit,

	k_number_of_model_permutation_flags
};

enum e_model_permutation_state_flags
{
	_model_permutation_state_blurred_bit,
	_model_permutation_state_hella_blurred_bit,
	_model_permutation_state_unshielded_bit,
	_model_permutation_state_battery_depleted_bit,
	_model_permutation_state_unknown4_bit,
	_model_permutation_state_unknown5_bit,
	_model_permutation_state_unknown6_bit,
	_model_permutation_state_unknown7_bit,

	k_number_of_model_permutation_state_flags
};

enum e_model_damage_state
{
	_model_damage_state_default,
	_model_damage_state_minor,
	_model_damage_state_medium,
	_model_damage_state_major,
	_model_damage_state_destroyed,

	k_number_of_model_damage_states
};

enum e_model_instance_group_choice
{
	_model_instance_group_choose_one_member,
	_model_instance_group_choose_all_members,

	k_number_of_model_instance_group_choices
};

enum e_model_material_type
{
	_model_material_dirt,
	_model_material_sand,
	_model_material_stone,
	_model_material_snow,
	_model_material_wood,
	_model_material_metalhollow,
	_model_material_metalthin,
	_model_material_metalthick,
	_model_material_rubber,
	_model_material_glass,
	_model_material_force_field,
	_model_material_grunt,
	_model_material_hunter_armor,
	_model_material_hunter_skin,
	_model_material_elite,
	_model_material_jackal,
	_model_material_jackal_energy_shield,
	_model_material_engineer_skin,
	_model_material_engineer_force_field,
	_model_material_flood_combat_form,
	_model_material_flood_carrier_form,
	_model_material_cyborg_armor,
	_model_material_cyborg_energy_shield,
	_model_material_human_armor,
	_model_material_human_skin,
	_model_material_sentinel,
	_model_material_monitor,
	_model_material_plastic,
	_model_material_water,
	_model_material_leaves,
	_model_material_elite_energy_shield,
	_model_material_ice,
	_model_material_hunter_shield,

	k_number_of_model_material_types
};

enum e_model_damage_flags
{
	_model_takes_shield_damage_for_children_bit,
	_model_takes_body_damage_for_children_bit,
	_model_always_shields_friendly_damage_bit,
	_model_passes_area_damage_to_children_bit,
	_model_parent_never_takes_body_damage_for_children_bit,
	_model_only_damaged_by_explosives_bit,
	_model_parent_never_takes_shield_damage_for_children_bit,
	_model_cannot_die_from_damage_bit,
	_model_passes_attached_damage_to_riders_bit,
	_model_shield_depletion_is_permanent_bit,
	_model_shield_depletion_force_hard_ping_bit,
	_model_ai_do_not_damage_without_player_bit,
	_model_health_regrows_while_dead_bit,
	_model_shield_recharge_plays_only_when_empty_bit,
	_model_ignore_force_minimum_transfer_bit,
	_model_orphan_from_postprocess_autogen_bit,
	_model_only_damaged_by_boarding_damage_bit,

	k_number_of_model_damage_flags
};

enum e_model_damage_section_flags
{
	_model_damage_section_absorbs_body_damage_bit,
	_model_damage_section_takes_full_damage_when_object_dies_bit,
	_model_damage_section_cannot_die_with_riders_bit,
	_model_damage_section_takes_full_damage_when_object_destroyed_bit,
	_model_damage_section_restored_on_ressurection_bit,
	_model_damage_section_unused5_bit,
	_model_damage_section_unused6_bit,
	_model_damage_section_headshotable_bit,
	_model_damage_section_ignores_shields_bit,
	_model_damage_section_takes_full_damage_when_shield_depleted_bit,
	_model_damage_section_networked_bit,
	_model_damage_section_allow_damage_response_overflow_bit,

	k_number_of_model_damage_section_flags
};

enum e_model_damage_section_instant_response_type
{
	_model_damage_section_receives_all_damage,
	_model_damage_section_receives_area_effect_damage,
	_model_damage_section_receives_local_damage,

	k_number_of_model_damage_section_instant_response_types
};

enum e_model_damage_section_constraint_type
{
	_model_damage_section_constraint_none,
	_model_damage_section_constraint_destroy_one_of_group,
	_model_damage_section_constraint_destroy_entire_group,
	_model_damage_section_constraint_loosen_one_of_group,
	_model_damage_section_constraint_loosen_entire_group,

	k_number_of_model_damage_section_constraint_types
};

enum e_model_damage_section_instant_response_flags
{
	_model_damage_section_response_kills_object_bit,
	_model_damage_section_response_inhibits_melee_attack_bit,
	_model_damage_section_response_inhibits_weapon_attack_bit,
	_model_damage_section_response_inhibits_walking_bit,
	_model_damage_section_response_forces_drop_weapon_bit,
	_model_damage_section_response_kills_weapon_primary_trigger_bit,
	_model_damage_section_response_kills_weapon_secondary_trigger_bit,
	_model_damage_section_response_destroys_object_bit,
	_model_damage_section_response_damages_weapon_primary_trigger_bit,
	_model_damage_section_response_damages_weapon_secondary_trigger_bit,
	_model_damage_section_response_light_damage_left_turn_bit,
	_model_damage_section_response_major_damage_left_turn_bit,
	_model_damage_section_response_light_damage_right_turn_bit,
	_model_damage_section_response_major_damage_right_turn_bit,
	_model_damage_section_response_light_damage_engine_bit,
	_model_damage_section_response_major_damage_engine_bit,
	_model_damage_section_response_kills_object_no_player_solo_bit,
	_model_damage_section_response_causes_detonation_bit,
	_model_damage_section_response_fires_on_creation_bit,
	_model_damage_section_response_kills_variant_objects_bit,
	_model_damage_section_response_force_unattached_effects_bit,
	_model_damage_section_response_fires_under_threshold_bit,
	_model_damage_section_response_triggers_special_death_bit,
	_model_damage_section_response_only_on_special_death_bit,
	_model_damage_section_response_only_not_on_special_death_bit,
	_model_damage_section_response_buckles_giants_bit,
	_model_damage_section_response_causes_sp_detonation_bit,
	_model_damage_section_response_skip_sounds_on_generic_effect_bit,
	_model_damage_section_response_kills_giants_bit,
	_model_damage_section_response_skip_sounds_on_special_death_bit,
	_model_damage_section_response_cause_head_dismemberment_bit,
	_model_damage_section_response_cause_left_leg_dismemberment_bit,

	k_number_of_model_damage_section_instant_response_flags
};

enum e_target_lock_on_flags
{
	_target_lock_on_headshot_bit,
	_target_lock_on_vulnerable_bit,
	_target_lock_on_ignored_on_local_physics_bit,
	_target_lock_on_use_for_network_lead_vector_only_bit,

	k_number_of_target_lock_on_flags
};

enum e_model_object_data_type
{
	_model_object_data_not_set,
	_model_object_data_user_defined,
	_model_object_data_auto_generated,

	k_number_of_model_object_data_types
};

enum e_model_shadow_override_type
{
	_model_shadow_override_prt_shadows_from_all_regions,
	_model_shadow_override_prt_self_shadow_only,
	_model_shadow_override_no_prt_shadows_at_all,

	k_number_of_model_shadow_override_types
};

struct s_model_permutation_state
{
	string_id permutation_name;
	char runtime_permutation_index;
	c_flags<e_model_permutation_state_flags, byte, k_number_of_model_permutation_state_flags> property_flags;
	c_enum<e_model_damage_state, short, k_number_of_model_damage_states> state;
	s_tag_reference looping_effect;
	string_id looping_effect_marker_name;
	real initial_probability;
};
static_assert(sizeof(s_model_permutation_state) == 0x20);

struct s_model_variant_permutation
{
	string_id permutation_name;
	char runtime_permutation_index;
	c_flags<e_model_permutation_flags, byte, k_number_of_model_permutation_flags> flags;
	short : 16;
	real probability;
	c_tag_block<s_model_permutation_state> states;
	char runtime_permutation_indices[k_maximum_number_of_model_region_permutations];
};
static_assert(sizeof(s_model_variant_permutation) == 0x24);

struct s_model_variant_region
{
	string_id region_name;
	char runtime_region_index;
	char runtime_flags;
	short parent_variant;
	c_tag_block<s_model_variant_permutation> permutations;
	c_enum<e_model_region_sort_order, long, k_number_of_model_region_sort_orders> sort_order;
};
static_assert(sizeof(s_model_variant_region) == 0x18);

struct s_model_variant_object
{
	string_id parent_marker;
	string_id child_marker;
	string_id child_variant_name;
	s_tag_reference child_object;
};
static_assert(sizeof(s_model_variant_object) == 0x1C);

struct s_model_variant
{
	string_id name;
	s_tag_reference voice;
	string_id dialog_effect;
	c_enum<e_model_ai_character, byte, k_number_of_model_ai_characters> ai_character;
	char : 8;
	char : 8;
	char : 8;
	char runtime_region_indices[k_maximum_number_of_model_regions];
	c_tag_block<s_model_variant_region> regions;
	c_tag_block<s_model_variant_object> objects;
	long instance_group_index;
	long : 32;
	long : 32;
};
static_assert(sizeof(s_model_variant) == 0x50);

struct s_model_region_sort
{
	string_id name;
};
static_assert(sizeof(s_model_region_sort) == 0x4);

struct s_model_instance_member
{
	long subgroup;
	string_id instances;
	real probability;
	long instance_placement_mask0;
	long instance_placement_mask1;
	long instance_placement_mask2;
	long instance_placement_mask3;
};
static_assert(sizeof(s_model_instance_member) == 0x1C);

struct s_model_instance_group
{
	string_id name;
	c_enum<e_model_instance_group_choice, long, k_number_of_model_instance_group_choices> choice;
	c_tag_block<s_model_instance_member> member_list;
	real total_probability;
};
static_assert(sizeof(s_model_instance_group) == 0x18);

struct s_model_material
{
	string_id name;
	c_enum<e_model_material_type, short, k_number_of_model_material_types> material_type; // unused?
	short damage_section;
	short runtime_collision_material_index;
	short runtime_damager_material_index;
	string_id global_material_name;
	short runtime_global_material_index;
	short : 16;
};
static_assert(sizeof(s_model_material) == 0x14);

struct s_model_permutation
{
	string_id name;
	uchar flags;
	char collision_permutation_index;
	char physics_permutation_index;
};
static_assert(sizeof(s_model_permutation) == 0x8);

struct s_model_region
{
	string_id name;
	char collision_region_index;
	char physics_region_index;
	c_tag_block<s_model_permutation> permutations;
};
static_assert(sizeof(s_model_region) == 0x14);

struct s_model_target
{
	long : 32; // c_flags<e_model_target_flags, byte, k_number_of_model_target_flags> flags; // unused padding outside of ED?
	string_id marker_name;
	real size;
	real cone_angle;
	short damage_section;
	short variant;
	float targeting_relevance;
	float aoe_exclusion_radius;
	// lock-on fields
	c_flags<e_target_lock_on_flags, byte, k_number_of_target_lock_on_flags> flags;
	byte : 8;
	byte : 8;
	byte : 8;
	real lock_on_distance;
	string_id tracking_type;
};
static_assert(sizeof(s_model_target) == 0x28);

struct s_runtime_node
{
	string_id name;
	short parent_node;
	short first_child_node;
	short next_sibling_node;
	short import_node_index;
	real_point3d default_translation;
	real_quaternion default_rotation;
	real_matrix4x3 default_inverse;
};
static_assert(sizeof(s_runtime_node) == 0x5C);

struct s_model_object_data_block
{
	c_enum<e_model_object_data_type, short, k_number_of_model_object_data_types> type;
	short : 16;
	real_point3d offset;
	real radius;
};
static_assert(sizeof(s_model_object_data_block) == 0x14);

struct s_scenario_load_parameter
{
	s_tag_reference scenario;
	s_tag_data parameters;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};
static_assert(sizeof(s_scenario_load_parameter) == 0x44);

struct s_shadow_cast_override
{
	string_id region;
	string_id shadow_cast_permutation;
};
static_assert(sizeof(s_shadow_cast_override) == 0x8);

struct s_shadow_receive_override
{
	string_id region;
	c_enum<e_model_shadow_override_type, long, k_number_of_model_shadow_override_types> shadow_type;
};
static_assert(sizeof(s_shadow_receive_override) == 0x8);

struct s_occlusion_sphere
{
	string_id marker_1_name;
	long marker_1_index;
	string_id marker_2_name;
	long marker_2_index;
	real radius;
};
static_assert(sizeof(s_occlusion_sphere) == 0x14);

struct s_model_definition
{
	s_tag_reference render_model;
	s_tag_reference collision_model;
	s_tag_reference animation;
	s_tag_reference physics_model;
	real reduce_to_lod_distances[k_number_of_model_lods];
	s_tag_reference lod_model;
	c_tag_block<s_model_variant> variants;
	c_tag_block<s_model_region_sort> region_sort;
	c_tag_block<s_model_instance_group> instance_groups;
	c_tag_block<s_model_material> materials;
	c_tag_block<s_global_damage_info_block> damage_info;
	c_tag_block<s_model_target> targets;
	c_tag_block<s_model_region> runtime_regions;
	c_tag_block<s_runtime_node> runtime_nodes;
	ulong runtime_nodes_checksum;
	c_tag_block<s_model_object_data_block> mode_object_data;
	s_tag_reference primary_dialog;
	s_tag_reference secondary_dialog;
	ulong flags;
	string_id default_dialog_effect;
	ulong render_only_node_flags[8]; // bit vector
	ulong render_section_flags[8]; // bit vector
	ulong runtime_flags;
	ulong unknown; // added in 11.1.498295 cert_ms29
	c_tag_block<s_scenario_load_parameter> scenario_load_parameters;
	char shadow_detail;
	char shadow_bounces;
	c_tag_block<s_shadow_cast_override> shadow_cast_overrides;
	c_tag_block<s_shadow_receive_override> shadow_receive_overrides;
	c_tag_block<s_occlusion_sphere> occlusion_spheres;
	s_tag_reference override_3p_shield_impact;
	s_tag_reference override_fp_shield_impact;
	s_tag_reference overshield_3p_shield_impact;
	s_tag_reference overshield_fp_shield_impact;
};
static_assert(sizeof(s_model_definition) == 0x1B8);