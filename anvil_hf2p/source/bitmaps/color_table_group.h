#pragma once
#include <tag_files\tag_groups.h>

struct s_color_table_color
{
	c_static_string<32> name;
	real_argb_color color;
};

struct color_table
{
	static tag const k_group_tag = COLOR_TABLE_TAG;

	c_typed_tag_block<s_color_table_color> colors;
};