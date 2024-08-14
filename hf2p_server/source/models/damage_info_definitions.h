#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_field.h>
#include <tag_files\tag_block.h>
#include <objects\damage.h>

enum e_global_damage_info_flags
{
	_global_damage_none_bit,
	_global_damage_takes_shield_damage_for_children_bit,
	_global_damage_takes_body_damage_for_children_bit,
	_global_damage_always_shields_friendly_damage_bit,
    _global_damage_passes_area_damage_to_children_bit,
    _global_damage_parent_never_takes_body_damage_for_children_bit,
    _global_damage_only_damaged_by_explosives_bit,
    _global_damage_parent_never_takes_shield_damage_for_children_bit,
    _global_damage_cannot_die_from_damage_bit,
    _global_damage_passes_attached_damage_to_riders_bit,
    _global_damage_shield_depletion_is_permanent_bit,
    _global_damage_shield_depletion_force_hard_ping_bit,
    _global_damage_ai_do_not_damage_without_player_bit,
    _global_damage_health_regrows_while_dead_bit,
    _global_damage_shield_recharge_plays_only_when_empty_bit,
    _global_damage_ignore_force_minimum_transfer_bit,
    _global_damage_orphan_from_postprocess_autogen_bit,
    _global_damage_only_damaged_by_boarding_damage_bit,

	k_number_of_global_damage_info_flags
};

enum e_damage_section_flags
{
    _damage_section_none_bit,
    _damage_section_absorbs_body_damage_bit,
    _damage_section_takes_full_damage_when_object_dies_bit,
    _damage_section_cannot_die_with_riders_bit,
    _damage_section_takes_full_damage_when_object_destroyed_bit,
    _damage_section_restored_on_ressurection_bit,
    _damage_section_unused0_bit,
    _damage_section_unused1_bit,
    _damage_section_headshotable_bit,
    _damage_section_ignores_shields_bit,
    _damage_section_takes_full_damage_when_shield_depleted_bit,
    _damage_section_networked_bit,
    _damage_section_allow_damage_response_overflow_bit,

    k_number_of_damage_section_flags
};

enum e_damage_response_type
{
    _damage_response_recieves_all_damage,
    _damage_response_recieves_area_effect_damage,
    _damage_response_recieves_local_damage,

    k_number_of_damage_response_types
};

enum e_damage_constraint_type
{
    _damage_response_none,
    _damage_response_destroy_one_of_group,
    _damage_response_destroy_entire_group,
    _damage_response_loosen_one_of_group,
    _damage_response_loosen_entire_group,

    k_number_of_damage_constraint_types
};

enum e_damage_response_flags
{
    _damage_response_none_bit,
    _damage_response_kills_object_bit,
    _damage_response_inhibits_melee_attack_bit,
    _damage_response_inhibits_weapon_attack_bit,
    _damage_response_inhibits_walking_bit,
    _damage_response_forces_drop_weapon_bit,
    _damage_response_kills_weapon_primary_trigger_bit,
    _damage_response_kills_weapon_secondary_trigger_bit,
    _damage_response_destroys_object_bit,
    _damage_response_damages_weapon_primary_trigger_bit,
    _damage_response_damages_weapon_secondary_trigger_bit,
    _damage_response_light_damage_left_turn_bit,
    _damage_response_major_damage_left_turn_bit,
    _damage_response_light_damage_right_turn_bit,
    _damage_response_major_damage_right_turn_bit,
    _damage_response_light_damage_engine_bit,
    _damage_response_major_damage_engine_bit,
    _damage_response_kills_object_no_player_solo_bit,
    _damage_response_causes_detonation_bit,
    _damage_response_fires_on_creation_bit,
    _damage_response_kills_variant_objects_bit,
    _damage_response_force_unattached_effects_bit,
    _damage_response_fires_under_threshold_bit,
    _damage_response_triggers_special_death_bit,
    _damage_response_only_on_special_death_bit,
    _damage_response_only_not_on_special_death_bit,
    _damage_response_buckles_giants_bit,
    _damage_response_causes_sp_detonation_bit,
    _damage_response_skip_sounds_on_generic_effect_bit,
    _damage_response_kills_giants_bit,
    _damage_response_skip_sounds_on_special_death_bit,
    _damage_response_cause_head_dismemberment_bit,
    _damage_response_cause_left_leg_dismemberment_bit,

    k_number_of_damage_response_flags
};

enum e_instant_response_state
{
    _instant_response_state_default,
    _instant_response_state_minor_damage,
    _instant_response_state_medium_damage,
    _instant_response_state_major_damage,
    _instant_response_state_destroyed,

    k_number_of_instant_response_states
};

enum e_custom_response_behavior
{
    _custom_response_plays_always,
    _custom_response_plays_if_labels_match,
    _custom_response_plays_if_labels_differ,

    k_number_of_custom_response_behaviors
};

enum e_runtime_constraint_type
{
    _runtime_constraint_none = -1,
    _runtime_constraint_hinge,
    _runtime_constraint_limited_hinge,
    _runtime_constraint_ragdoll,
    _runtime_constraint_stiff_spring,
    _runtime_constraint_ball_and_socket,
    _runtime_constraint_prismatic,

    k_number_of_runtime_constraint_types
};

struct s_global_damage_info_body
{
    real maximum_vitality;
    real minimum_stun_damage;
    real stun_time;
    real recharge_time;
    real recharge_fraction;
    byte unknown_runtime_values[64];
};
static_assert(sizeof(s_global_damage_info_body) == 0x54);

struct s_global_damage_info_shield
{
    real maximum_shield_vitality;
    string_id global_shield_material_name;
    real minimum_stun_damage;
    real stun_time;
    real recharge_time;
    real shield_damaged_threshold;
    c_typed_tag_reference<EFFECT_TAG> shield_damaged_effect;
    c_typed_tag_reference<EFFECT_TAG> shield_depleted_effect;
    c_typed_tag_reference<EFFECT_TAG> shield_recharging_effect;
};
static_assert(sizeof(s_global_damage_info_shield) == 0x48);

struct s_instant_response
{
    c_enum<e_damage_response_type, short, _damage_response_recieves_all_damage, k_number_of_damage_response_types> response_type;
    // Constraint damage type
    // * if you specify a constraint group name (see lower section of this block) you can specify a contraint damage
    // * loosening a constraint takes it out of the rigid state - activates it
    // * destroying a constraint sets the attached body free
    c_enum<e_damage_constraint_type, short, _damage_response_none, k_number_of_damage_constraint_types> constraint_damage_type;
    // Constraint destruction
    // - a response can destroy a single constraint by naming it explicitly.
    // - alternatively it can randomly destroy a single constraint from a specified group if the "destroy one group constraint" flag is set
    // - also it can destroy all constraints in a specified group if the "destroy all group constraints" flag is set
    string_id contraint_or_group_name;
    // Damage response flags
    // * kills obeject: when the response fires the object dies regardless of its current health
    // * inhibits <x>: from halo1 - disallows basic behaviours for a unit
    // * forces drop weapon: from halo1 - makes the unit drop its current weapon
    // * kills weapon <x> trigger: destroys the <x> trigger on the unit's current weapon
    // * destroys object: when the response fires the object is destroyed
    c_flags<e_damage_response_flags, long, k_number_of_damage_response_flags> flags;
    real damage_threshold;
    c_typed_tag_reference<EFFECT_TAG, DAMAGE_EFFECT_TAG> transition_effect_generic;
    c_typed_tag_reference<EFFECT_TAG, DAMAGE_EFFECT_TAG> transition_effect_specific;
    c_typed_tag_reference<EFFECT_TAG, DAMAGE_EFFECT_TAG> transition_effect_damage;
    string_id region;
    c_enum<e_instant_response_state, short, _instant_response_state_default, k_number_of_instant_response_states> new_state;
    short runtime_region_index;
    string_id region_secondary;
    c_enum<e_instant_response_state, short, _instant_response_state_default, k_number_of_instant_response_states> new_state_secondary;
    short runtime_region_index_secondary;
    short destroy_instance_group;
    c_enum<e_custom_response_behavior, short, _custom_response_plays_always, k_number_of_custom_response_behaviors> custom_response_behavior;
    string_id custom_response_label;
    string_id effect_marker_name;
    string_id damage_effect_marker_name;
    // Response delay
    // If desired, you can specify a delay until the response fires. This delay is pre-empted if another timed response for
    // the same section fires. The delay effect plays while the timer is counting down
    real response_delay;
    c_typed_tag_reference<EFFECT_TAG> delay_effect;
    string_id delay_effect_marker_name;
    // seat ejaculation
    string_id ejecting_seat_label;
    // skip fraction
    // 0.0 always fires, 1.0 never fires
    real skip_fraction;
    // destroyed child object marker name
    // when this response fires, any children objects created at the supplied marker name will be destroyed
    string_id destroyed_child_object_marker_name;
    // total damage threshold
    // scale on total damage section vitality
    real total_damage_threshold;
};
static_assert(sizeof(s_instant_response) == 0x88);

struct s_damage_section
{
    string_id name;
    // * absorbs body damage: damage to this section does not count against body vitality
    // * headshottable: takes extra headshot damage when shot
    // * ignores shields: damage to this section bypasses shields
    c_flags<e_damage_section_flags, long, k_number_of_damage_section_flags> flags;
    real vitality_percentage;
    c_typed_tag_block<s_instant_response> instant_responses;
    c_typed_tag_block<s_tag_block_stub> unused0;
    c_typed_tag_block<s_tag_block_stub> unused1;
    real stun_time;
    real recharge_time;
    real runtime_recharge_velocity;
    string_id resurrection_restored_region_name;
    short runtime_resurrection_restored_region_index;
};
static_assert(sizeof(s_damage_section) == 0x44);

struct s_damage_node
{
    short runtime_damage_part;
    short : 16;
    long : 32;
    long : 32;
    long : 32;
};
static_assert(sizeof(s_damage_node) == 0x10);

struct s_region_specific_damage_block
{
    // region-state-specific-damage
    // for fields below, 0.0==inherit from damage seat
    string_id damage_region_name;
    short runtime_damage_region_index;
    short : 16;
    real direct_damage_scale_minor;
    real max_transfer_scale_minor;
    real min_transfer_scale_minor;
    real direct_damage_scale_medium;
    real max_transfer_scale_medium;
    real min_transfer_scale_medium;
    real direct_damage_scale_major;
    real max_transfer_scale_major;
    real min_transfer_scale_major;
};
static_assert(sizeof(s_region_specific_damage_block) == 0x2C);

struct s_damage_seat
{
    string_id seat_label;
    real direct_damage_scale;
    real damage_transfer_fall_off_radius;
    real maximum_transfer_damage_scale;
    real minimum_transfer_damage_scale;
    c_typed_tag_block<s_region_specific_damage_block> region_specific_damage;
};
static_assert(sizeof(s_damage_seat) == 0x20);

struct s_damage_constraint
{
    string_id physics_model_constraint_name;
    string_id damage_constraint_name;
    string_id damage_constraint_group_name;
    real group_probability_scale;
    c_enum<e_runtime_constraint_type, short, _runtime_constraint_none, k_number_of_runtime_constraint_types> runtime_constraint_type;
    short runtime_constraint_index;
};
static_assert(sizeof(s_damage_constraint) == 0x14);

struct s_global_damage_info_block
{
	c_flags<e_global_damage_info_flags, long, k_number_of_global_damage_info_flags> flags;
    string_id global_indirect_material_name;
    short indirect_damage_section;
    short : 16;
    long : 32;
    c_enum<e_damage_reporting_type, char, _damage_reporting_type_unknown, k_damage_reporting_type_count> collision_damage_reporting_type;
    c_enum<e_damage_reporting_type, char, _damage_reporting_type_unknown, k_damage_reporting_type_count> response_damage_reporting_type;
    short : 16;
    long : 32;
    long : 32;
    long : 32;
    long : 32;
    long : 32;
    s_global_damage_info_body body;
    s_global_damage_info_shield shield;
    c_typed_tag_block<s_damage_section> damage_sections;
    c_typed_tag_block<s_damage_node> nodes;
    short runtime_shield_material_type;
    short runtime_indirect_material_type;
    real runtime_shield_recharge_velocity;
    real runtime_health_recharge_velocity;
    c_typed_tag_block<s_damage_seat> damage_seats;
    c_typed_tag_block<s_damage_constraint> damage_constraints;
};
static_assert(sizeof(s_global_damage_info_block) == 0x100);