#pragma once
#include "..\cseries\cseries.h"
#include "tag_field.h"

enum e_tag_data_flags
{
	k_number_of_tag_data_flags
};

struct s_tag_block_definition
{
	const char* name;
	long element_size;
	long maximum_element_count;
	s_tag_field_definition* fields;
};
static_assert(sizeof(s_tag_block_definition) == 0x10);

template<typename t_block_type>
class c_tag_block
{
public:
	long count;
	t_block_type* address;
	s_tag_block_definition* definition;
};

struct s_tag_data
{
	long size;
	c_flags<e_tag_data_flags, long, k_number_of_tag_data_flags> flags;
	long stream_position;
	void* address;
	s_tag_block_definition* definition;
};
static_assert(sizeof(s_tag_data) == 0x14);