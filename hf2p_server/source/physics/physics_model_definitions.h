#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_block.h>
#include <tag_files\tag_field.h>

enum e_physics_model_flags
{
	_physics_model_flags_serialized_havok_data_bit,
	_physics_model_flags_make_physical_children_keyframed_bit,
	_physics_model_flags_shrink_radius_by_havok_complex_radius_bit,
	_physics_model_flags_use_physics_for_collision_bit,

	k_number_of_physics_model_flags
};

// TODO: finish block defs
struct s_physics_model_definition
{
	c_flags<e_physics_model_flags, long, k_number_of_physics_model_flags> flags;
	real mass;
	real low_frequency_deactivation_scale;
	real high_frequency_deactivation_scale;
	real custom_shape_radius;
	real maximum_penetration_depth_scale;
	byte import_version;
	c_tag_block<void> damped_spring_motors;
	c_tag_block<void> position_motors;
	c_tag_block<void> phantom_types;
	c_tag_block<void> powered_chains;
	// WARNING WARNING WARNING
	// Do not add or delete any elements from the following block
	c_tag_block<void> node_edges;
	c_tag_block<void> rigid_bodies;
	c_tag_block<void> materials;
	c_tag_block<void> spheres;
	c_tag_block<void> multi_spheres;
	c_tag_block<void> pills;
	c_tag_block<void> boxes;
	c_tag_block<void> triangles;
	c_tag_block<void> polyhedra;
	// ALL THESE WORLDS ARE YOURS, EXCEPT EUROPA...
	// Attempt no landings there. And you can't edit anything below this point, so why even look at it?
	c_tag_block<void> polyhedron_four_vectors;
	c_tag_block<void> polyhedron_plane_equations;
	c_tag_block<void> mass_distributions;
	c_tag_block<void> lists;
	c_tag_block<void> list_shapes;
	c_tag_block<void> mopps;
	s_tag_data mopp_codes;
	c_tag_block<void> hinge_constraints;
	c_tag_block<void> ragdoll_constraints;
	c_tag_block<void> regions;
	c_tag_block<void> nodes;
	c_tag_block<void> errors;
	c_tag_block<void> point_to_path_curves;
	c_tag_block<void> limited_hinge_constraints;
	c_tag_block<void> ball_and_socket_constraints;
	c_tag_block<void> stiff_spring_constraints;
	c_tag_block<void> prismatic_constraints;
	c_tag_block<void> phantoms;
};
static_assert(sizeof(s_physics_model_definition) == 0x198);