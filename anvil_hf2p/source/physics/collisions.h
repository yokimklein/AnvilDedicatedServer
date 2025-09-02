#pragma once
#include <cseries\cseries.h>
#include <physics\collision_bsp_references.h>
#include <game\materials.h>

enum e_collision_result_type
{
	_collision_result_none = 0,
	_collision_result_structure,
	_collision_result_water,
	_collision_result_instanced_geometry,
	_collision_result_object,

	k_collision_result_type_count
};

struct collision_result
{
	collision_result() :
		type(_collision_result_none),
		t(),
		position(),
		start_location(),
		location(),
		material_type(c_global_material_type()),
		__pad22(),
		plane(),
		instanced_geometry_instance_index(),
		object_index(),
		region_index(),
		node_index(),
		bsp_reference(),
		bsp_index(),
		leaf_index(),
		surface_index(),
		plane_designator(),
		edge_index(),
		flags(),
		breakable_surface_index(),
		__unknown57(),
		material_index(),
		breakable_surface_set_index(),
		__data5B()
	{
	}

	c_enum<e_collision_result_type, long, _collision_result_none, k_collision_result_type_count> type;
	real t;
	real_point3d position;
	s_location start_location;
	s_location location;
	c_collision_bsp_reference collision_bsp_reference;
	c_global_material_type material_type;
	byte __pad22[0x2];
	real_plane3d plane;
	long instanced_geometry_instance_index;
	datum_index object_index;
	short region_index;
	short node_index;
	dword bsp_reference;
	long bsp_index;
	long leaf_index;
	long surface_index;
	long plane_designator;
	char edge_index;
	byte_flags flags; // c_flags<e_surface_flags, byte, k_surface_flags>
	byte breakable_surface_index;
	byte __unknown57;
	short material_index;
	char breakable_surface_set_index;
	byte __data5B[0x1];
};
static_assert(sizeof(collision_result) == 0x5C);