#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_field.h>
#include <tag_files\tag_block.h>
#include <objects\damage.h>
#include <tag_files\tag_field.h>
#include <tag_files\tag_block.h>
#include <objects\multiplayer_game_objects.h>

enum e_object_definition_flags
{
	_object_does_not_cast_shadow_bit,
	_object_search_cardinal_direction_lightmaps_bit,
	_object_preserves_initial_damage_owner_bit,
	_object_not_a_pathfinding_obstacle_bit,
	_object_extension_of_parent_bit,
	_object_does_not_cause_collision_damage_bit,
	_object_early_mover_bit,
	_object_early_mover_localized_physics_bit,
	_object_use_static_massive_lightmap_sample_bit,
	_object_scales_attachments_bit,
	_object_inherits_player_appearance_bit,
	_object_dead_bipeds_cannot_localize_bit,
	_object_attach_to_clusters_by_dynamic_sphere_bit,
	_object_effects_do_not_spawn_objects_in_multiplayer_bit,
	_object_does_not_collide_with_camera_bit,
	_object_damage_not_blocked_by_obstructions,

	k_number_of_object_definition_flags
};

enum e_object_lightmap_shadow_mode
{
	_object_lightmap_shadow_mode_default,
	_object_lightmap_shadow_mode_never,
	_object_lightmap_shadow_mode_always,
	_object_lightmap_shadow_mode_blur,

	k_number_of_object_lightmap_shadow_modes
};

enum e_object_sweetener_size
{
	_object_sweetener_size_small,
	_object_sweetener_size_medium,
	_object_sweetener_size_large,

	k_number_of_object_sweetener_sizes
};

enum e_object_water_density
{
	_object_water_density_default,
	_object_water_density_least,
	_object_water_density_some,
	_object_water_density_equal,
	_object_water_density_more,
	_object_water_density_more_still,
	_object_water_density_lots_more,

	k_number_of_object_water_densities
};

enum e_object_definition_runtime_flags
{
	_object_definition_runtime_unknown0_bit,
	_object_definition_runtime_unknown1_bit,
	_object_definition_runtime_unknown2_bit,
	_object_definition_runtime_unknown3_bit,
	_object_definition_runtime_unknown4_bit,
	_object_definition_runtime_unknown5_bit,
	_object_definition_runtime_unknown6_bit,
	_object_definition_runtime_unknown7_bit,
	_object_definition_runtime_unknown8_bit,
	_object_definition_runtime_unknown9_bit,
	_object_definition_runtime_unknown10_bit,
	_object_definition_runtime_unknown11_bit,
	_object_definition_runtime_unknown12_bit,
	_object_definition_runtime_unknown13_bit,
	_object_definition_runtime_unknown14_bit,
	_object_definition_runtime_unknown15_bit,
	_object_definition_runtime_unknown16_bit,
	_object_definition_runtime_unknown17_bit,
	_object_definition_runtime_unknown18_bit,
	_object_definition_runtime_unknown19_bit,
	_object_definition_runtime_unknown20_bit,
	_object_definition_runtime_unknown21_bit,
	_object_definition_runtime_unknown22_bit,
	_object_definition_runtime_unknown23_bit,
	_object_definition_runtime_unknown24_bit,
	_object_definition_runtime_unknown25_bit,
	_object_definition_runtime_unknown26_bit,
	_object_definition_runtime_unknown27_bit,
	_object_definition_runtime_unknown28_bit,
	_object_definition_runtime_unknown29_bit,
	_object_definition_runtime_unknown30_bit,
	_object_definition_runtime_unknown31_bit,

	k_number_of_object_definition_runtime_flags
};

enum e_object_change_color
{
	_object_change_color_primary,
	_object_change_color_secondary,
	_object_change_color_tertiary,
	_object_change_color_quaternary,
	_object_change_color_quinary,

	k_number_of_object_change_colors
};

enum e_object_ai_properties_flags
{
	_object_ai_properties_destroyable_cover_bit,
	_object_ai_properties_pathfinding_ignore_when_dead_bit,
	_object_ai_properties_dynamic_cover_bit,
	_object_ai_properties_non_flight_blocking_bit,
	_object_ai_properties_dynamic_cover_from_center_bit,
	_object_ai_properties_has_corner_markers_bit,
	_object_ai_properties_idle_when_flying_bit,

	k_number_of_object_ai_properties_flags
};

enum e_object_ai_size
{
	_object_ai_size_default,
	_object_ai_size_tiny,
	_object_ai_size_small,
	_object_ai_size_medium,
	_object_ai_size_large,
	_object_ai_size_huge,
	_object_ai_size_immobile,

	k_number_of_object_ai_sizes
};

enum e_object_ai_distance
{
	_object_ai_distance_none,
	_object_ai_distance_down,
	_object_ai_distance_step,
	_object_ai_distance_crouch,
	_object_ai_distance_stand,
	_object_ai_distance_storey,
	_object_ai_distance_tower,
	_object_ai_distance_infinite,

	k_number_of_object_ai_distances
};

enum e_object_function_flags
{
	_object_function_invert_bit,
	_object_function_mapping_does_not_control_active_bit,
	_object_function_always_active_bit,
	_object_function_random_time_offset_bit,
	_object_function_always_exports_value_bit,
	_object_function_turn_off_with_uses_magnitude_bit,

	k_number_of_object_function_flags
};

enum e_object_attachment_atlas_flags
{
	_object_attachment_atlas_gameplay_vision_mode_bit,
	_object_attachment_atlas_theater_vision_mode_bit,

	k_number_of_object_attachment_atlas_flags
};

enum e_object_attachment_change_color
{
	_object_attachment_change_color_none,
	_object_attachment_change_color_primary,
	_object_attachment_change_color_secondary,
	_object_attachment_change_color_tertiary,
	_object_attachment_change_color_quaternary,

	k_number_of_object_attachment_change_colors
};

enum e_object_attachment_flags
{
	_object_attachment_force_always_on_bit,
	_object_attachment_effect_size_scale_from_object_scale_bit,

	k_number_of_object_attachment_flags
};

enum e_object_change_color_function_flags
{
	_object_change_color_function_blend_in_hsv_bit,
	_object_change_color_function_more_colors_bit,

	k_number_of_object_change_color_function_flags
};

enum e_object_model_data_type
{
	_object_model_data_not_set,
	_object_model_data_user_defined,
	_object_model_data_auto_generated,

	k_number_of_object_model_data_types
};

struct s_object_early_mover_obb_definition
{
	string_id name;
	real_bounds x_bounds;
	real_bounds y_bounds;
	real_bounds z_bounds;
	real_euler_angles3d angles;
};
static_assert(sizeof(s_object_early_mover_obb_definition) == 0x28);

struct s_object_ai_properties_definition
{
	c_flags<e_object_ai_properties_flags, long, k_number_of_object_ai_properties_flags> flags;
	string_id ai_type_name;
	c_enum<e_object_ai_size, short, _object_ai_size_default, k_number_of_object_ai_sizes> size;
	c_enum<e_object_ai_distance, short, _object_ai_distance_none, k_number_of_object_ai_distances> leap_jump_speed;
};
static_assert(sizeof(s_object_ai_properties_definition) == 0xC);

struct s_object_function_definition
{
	c_flags<e_object_function_flags, long, k_number_of_object_function_flags> flags;
	string_id import_name;
	string_id export_name;
	string_id turn_off_with;
	real minimum_value;
	s_tag_data default_function;
	string_id scale_by;
};
static_assert(sizeof(s_object_function_definition) == 0x2C);

struct s_object_attachment_definition
{
	c_flags<e_object_attachment_atlas_flags, long, k_number_of_object_attachment_atlas_flags> atlas_flags;
	c_typed_tag_reference<LIGHT_TAG, EFFECT_TAG, SOUND_LOOPING_TAG, LENS_FLARE_TAG> type;
	string_id marker;
	c_enum<e_object_attachment_change_color, short, _object_attachment_change_color_none, k_number_of_object_attachment_change_colors> change_color;
	c_flags<e_object_attachment_flags, word, k_number_of_object_attachment_flags> flags;
	string_id primary_scale;
	string_id secondary_scale;
};
static_assert(sizeof(s_object_attachment_definition) == 0x24);

struct s_object_widget_definition
{
	c_typed_tag_reference<ANTENNA_TAG, CLOTH_TAG, LEAF_SYSTEM_TAG, CELLULAR_AUTOMATA_TAG, CELLULAR_AUTOMATA2D_TAG> type;
};
static_assert(sizeof(s_object_widget_definition) == 0x10);

struct s_object_change_color_permutation_definition
{
	real weight;
	real_rgb_color color_lower_bound;
	real_rgb_color color_upper_bound;
	string_id variant_name;
};
static_assert(sizeof(s_object_change_color_permutation_definition) == 0x20);

struct s_object_change_color_function_definition
{
	long : 32;
	c_flags<e_object_change_color_function_flags, long, k_number_of_object_change_color_function_flags> scale_flags;
	real_rgb_color color_lower_bound;
	real_rgb_color color_upper_bound;
	string_id darken_by;
	string_id scale_by;
};
static_assert(sizeof(s_object_change_color_function_definition) == 0x28);

struct s_object_change_color_definition
{
	c_typed_tag_block<s_object_change_color_permutation_definition> initial_permutations;
	c_typed_tag_block<s_object_change_color_function_definition> functions;
};
static_assert(sizeof(s_object_change_color_definition) == 0x18);

struct s_object_node_map_definition
{
	byte target_node;
};
static_assert(sizeof(s_object_node_map_definition) == 0x1);

struct s_object_health_pack_definition
{
	c_typed_tag_reference<EQUIPMENT_TAG> equipment;
};
static_assert(sizeof(s_object_health_pack_definition) == 0x10);

struct s_object_model_data_definition
{
	c_enum<e_object_model_data_type, short, _object_model_data_not_set, k_number_of_object_model_data_types> type;
	short : 16;
	real_point3d offset;
	real radius;
};
static_assert(sizeof(s_object_model_data_definition) == 0x14);

struct object_definition
{
	c_enum<e_object_type, short, _object_type_biped, k_object_type_count> type;
	c_flags<e_object_definition_flags, short, k_number_of_object_definition_flags> object_flags;
	real bounding_radius;
	real_point3d bounding_offset;
	real acceleration_scale;
	c_enum<e_object_lightmap_shadow_mode, short, _object_lightmap_shadow_mode_default, k_number_of_object_lightmap_shadow_modes> lightmap_shadow_mode;
	c_enum<e_object_sweetener_size, byte, _object_sweetener_size_small, k_number_of_object_sweetener_sizes> sweetener_size;
	c_enum<e_object_water_density, byte, _object_water_density_default, k_number_of_object_water_densities> water_density;
	c_enum<e_object_definition_runtime_flags, long, _object_definition_runtime_unknown0_bit, k_number_of_object_definition_runtime_flags> runtime_flags;
	real dynamic_light_sphere_radius;
	real_point3d dynamic_light_sphere_offset;
	string_id default_model_variant;
	c_typed_tag_reference<MODEL_TAG> model;
	c_typed_tag_reference<CRATE_TAG> crate_object;
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> collision_damage;
	c_typed_tag_block<s_object_early_mover_obb_definition> early_mover_obb;
	c_typed_tag_reference<EFFECT_TAG> creation_effect;
	c_typed_tag_reference<MATERIAL_EFFECTS_TAG> material_effects;
	c_typed_tag_reference<ARMOR_SOUNDS_TAG> armor_sounds;
	c_typed_tag_reference<SOUND_TAG> melee_sound;
	c_typed_tag_block<s_object_ai_properties_definition> ai_properties;
	c_typed_tag_block<s_object_function_definition> functions;
	short hud_text_message_index;
	short : 16;
	c_typed_tag_block<s_object_attachment_definition> attachments;
	c_typed_tag_block<s_object_widget_definition> widgets;
	c_typed_tag_block<s_object_change_color_definition> change_colors;
	c_typed_tag_block<s_object_node_map_definition> node_maps;
	c_typed_tag_block<s_multiplayer_object_definition> multiplayer_object;
	c_typed_tag_block<s_object_health_pack_definition> health_packs;
	c_typed_tag_block<s_object_model_data_definition> model_data;
};
static_assert(sizeof(object_definition) == 0x120);