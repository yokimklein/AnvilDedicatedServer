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
	c_typed_tag_block<s_tag_block_stub> damped_spring_motors;
	c_typed_tag_block<s_tag_block_stub> position_motors;
	c_typed_tag_block<s_tag_block_stub> phantom_types;
	c_typed_tag_block<s_tag_block_stub> powered_chains;
	// WARNING WARNING WARNING
	// Do not add or delete any elements from the following block
	c_typed_tag_block<s_tag_block_stub> node_edges;
	c_typed_tag_block<s_tag_block_stub> rigid_bodies;
	c_typed_tag_block<s_tag_block_stub> materials;
	c_typed_tag_block<s_tag_block_stub> spheres;
	c_typed_tag_block<s_tag_block_stub> multi_spheres;
	c_typed_tag_block<s_tag_block_stub> pills;
	c_typed_tag_block<s_tag_block_stub> boxes;
	c_typed_tag_block<s_tag_block_stub> triangles;
	c_typed_tag_block<s_tag_block_stub> polyhedra;
	// ALL THESE WORLDS ARE YOURS, EXCEPT EUROPA...
	// Attempt no landings there. And you can't edit anything below this point, so why even look at it?
	c_typed_tag_block<s_tag_block_stub> polyhedron_four_vectors;
	c_typed_tag_block<s_tag_block_stub> polyhedron_plane_equations;
	c_typed_tag_block<s_tag_block_stub> mass_distributions;
	c_typed_tag_block<s_tag_block_stub> lists;
	c_typed_tag_block<s_tag_block_stub> list_shapes;
	c_typed_tag_block<s_tag_block_stub> mopps;
	s_tag_data mopp_codes;
	c_typed_tag_block<s_tag_block_stub> hinge_constraints;
	c_typed_tag_block<s_tag_block_stub> ragdoll_constraints;
	c_typed_tag_block<s_tag_block_stub> regions;
	c_typed_tag_block<s_tag_block_stub> nodes;
	c_typed_tag_block<s_tag_block_stub> errors;
	c_typed_tag_block<s_tag_block_stub> point_to_path_curves;
	c_typed_tag_block<s_tag_block_stub> limited_hinge_constraints;
	c_typed_tag_block<s_tag_block_stub> ball_and_socket_constraints;
	c_typed_tag_block<s_tag_block_stub> stiff_spring_constraints;
	c_typed_tag_block<s_tag_block_stub> prismatic_constraints;
	c_typed_tag_block<s_tag_block_stub> phantoms;
};
static_assert(sizeof(s_physics_model_definition) == 0x198);