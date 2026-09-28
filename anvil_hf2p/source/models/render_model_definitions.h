#pragma once
#include <tag_files\tag_field.h>
#include <tag_files\tag_block.h>
#include <tag_files\tag_groups.h>
#include <math\real_math.h>

struct s_render_model_marker
{
	char region_index;
	char permutation_index;
	char node_index;
	char : 8;
	real_point3d translation;
	real_quaternion rotation;
	real scale;
};
static_assert(sizeof(s_render_model_marker) == 0x24);

struct s_render_model_marker_group
{
	string_id name;
	c_typed_tag_block<s_render_model_marker> markers;
};
static_assert(sizeof(s_render_model_marker_group) == 0x10);

struct render_model_definition
{
	static tag const k_group_tag = RENDER_MODEL_TAG;

	byte __data0[0x3C];
	c_typed_tag_block<s_render_model_marker_group> marker_groups;
};
