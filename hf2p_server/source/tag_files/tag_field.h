#pragma once
#include "..\memory\data.h"

enum e_tag_field_type
{
	_field_tag,
	_field_string,
	_field_long_string,
	_field_string_id,
	_field_char_integer,
	_field_short_integer,
	_field_long_integer,
	_field_int64_integer,
	_field_byte_integer,
	_field_word_integer,
	_field_dword_integer,
	_field_qword_integer,
	_field_char_enum,
	_field_short_enum,
	_field_long_enum,
	_field_byte_flags,
	_field_word_flags,
	_field_long_flags,
	_field_point2d,
	_field_rectangle2d,
	_field_rgb_color,
	_field_argb_color,
	_field_short_bounds,
	_field_angle,
	_field_real,
	_field_real_fraction,
	_field_real_point2d,
	_field_real_point3d,
	_field_real_vector2d,
	_field_real_vector3d,
	_field_real_quaternion,
	_field_real_euler_angles2d,
	_field_real_euler_angles3d,
	_field_real_plane2d,
	_field_real_plane3d,
	_field_real_rgb_color,
	_field_real_argb_color,
	_field_real_hsv_color,
	_field_real_ahsv_color,
	_field_angle_bounds,
	_field_real_bounds,
	_field_real_fraction_bounds,
	_field_tag_reference,
	_field_block,
	_field_char_block_index,
	_field_short_block_index,
	_field_long_block_index,
	_field_byte_block_flags,
	_field_word_block_flags,
	_field_long_block_flags,
	_field_data,
	_field_array,
	_field_struct,
	_field_pointer,
	_field_skip,
	_field_padding,
	_field_explanation,
	_field_terminator,

	k_number_of_tag_field_types
};

struct s_tag_field_definition
{
	c_enum<e_tag_field_type, long, k_number_of_tag_field_types> type;
	const char* name;
	void* definition;
};
static_assert(sizeof(s_tag_field_definition) == 0xC);

struct s_tag_reference
{
	tag group_tag;
	const char* name;
	long name_length;
	datum_index index;
};
static_assert(sizeof(s_tag_reference) == 0x10);