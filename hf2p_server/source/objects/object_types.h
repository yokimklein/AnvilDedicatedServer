#pragma once
#include <memory\data.h>
#include <cseries\cseries.h>

enum e_object_type
{
	_object_type_biped,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_armor,
	_object_type_equipment,
	_object_type_arg_device,
	_object_type_terminal,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_sound_scenery,
	_object_type_crate,
	_object_type_creature,
	_object_type_giant,
	_object_type_effect_scenery,

	k_object_type_count
};

enum e_object_mask
{
	_object_mask_object = NONE,
	_object_mask_biped = FLAG(_object_type_biped),
	_object_mask_vehicle = FLAG(_object_type_vehicle),
	_object_mask_weapon = FLAG(_object_type_weapon),
	_object_mask_armor = FLAG(_object_type_armor),
	_object_mask_equipment = FLAG(_object_type_equipment),
	_object_mask_arg_device = FLAG(_object_type_arg_device),
	_object_mask_terminal = FLAG(_object_type_terminal),
	_object_mask_projectile = FLAG(_object_type_projectile),
	_object_mask_scenery = FLAG(_object_type_scenery),
	_object_mask_machine = FLAG(_object_type_machine),
	_object_mask_control = FLAG(_object_type_control),
	_object_mask_sound_scenery = FLAG(_object_type_sound_scenery),
	_object_mask_crate = FLAG(_object_type_crate),
	_object_mask_creature = FLAG(_object_type_creature),
	_object_mask_giant = FLAG(_object_type_giant),
	_object_mask_effect_scenery = FLAG(_object_type_effect_scenery),

	_object_mask_item = (_object_mask_weapon | _object_mask_equipment),
	_object_mask_unit = (_object_mask_biped | _object_mask_vehicle | _object_mask_giant),
	_object_mask_physics = (_object_mask_biped | _object_mask_vehicle | _object_mask_weapon | _object_mask_equipment | _object_mask_arg_device | _object_mask_terminal | _object_mask_scenery | _object_mask_machine | _object_mask_crate | _object_mask_creature | _object_mask_giant)
};

struct s_object_placement_data;
struct s_scenario_object;
class c_mover_definition_data;

struct object_type_definition
{
	const char* name;
	tag group_tag;
	short datum_size;
	short placement_tag_block_offset;
	short palette_tag_block_offset;
	short placement_tag_block_element_size;
	long maximum_placement_count;
	long mixing_board_index;
	c_mover_definition_data* mover_definition;
	void(*initialize)(object_type_definition*, long);
	void(*dispose)(void);
	void(*initialize_for_new_map)(void);
	void(*dispose_from_old_map)(void);
	void(*initialize_for_new_structure_bsp)(long);
	void(*disconnect_from_structure_bsp)(long);
	void(*adjust_placement)(s_object_placement_data*);
	bool(*new_)(long, s_object_placement_data*, bool*);
	void(*place)(long, s_scenario_object*);
	void(*unplace)(long);
	void(*create_children)(long);
	void(*delete_)(long);
	long(*export_function_values_A)(long);
	long(*export_function_values_B)(long);
	void(*move)(long);
	bool(*compute_activation)(long, long, long);
	bool(*compute_function_value)(long, string_id, long, real*, bool*, long);
	void(*attach_simulation_gamestate_index)(long);
	void(*detach_simulation_gamestate_index)(long);
	void(*attach_to_marker)(long, long, long);
	void(*attach_to_node)(long, long, long);
	void(*detach_from_parent)(long);
	void(*handle_deleted_object)(long, long);
	ulong handle_deleted_object_flags;
	void(*handle_deleted_player)(long, long);
	void(*unused)(long, long, long);
	bool(*handle_parent_destroyed)(long);
	void(*fix_transform)(long, real_vector3d*, real_vector3d*, real_vector3d*);
	void(*fix_transform_to_physics)(long, real_matrix4x3*);
	void(*fix_transform_from_physics)(long, real_matrix4x3*);
	void(*preprocess_node_orientations)(long, long, long, long);
	void(*preprocess_root_node_matrix)(long, real_matrix4x3*);
	void(*postprocess_node_matrices)(long, long, real_matrix4x3*);
	void(*reset)(long);
	void(*notify_impulse_sound)(long, long, long);
	void(*render_debug)(long);
	void(*unknown)(long);
	object_type_definition* child_types[k_object_type_count];
	object_type_definition* next_type;
	ulong deleted_object_handler_mask;
};
static_assert(sizeof(object_type_definition) == 0xF8);

extern object_type_definition* (&object_type_definitions)[k_object_type_count];

void object_type_detach_gamestate_entity(datum_index object_index);
object_type_definition* object_type_definition_get(e_object_type object_type);