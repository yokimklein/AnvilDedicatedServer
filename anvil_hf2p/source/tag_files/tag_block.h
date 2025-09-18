#pragma once
#include <cseries\cseries.h>
#include <tag_files\tag_field.h>

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

struct s_tag_data
{
	long size;
	c_flags<e_tag_data_flags, long, k_number_of_tag_data_flags> flags;
	long stream_position;
	void* address;
	s_tag_block_definition* definition;
};
static_assert(sizeof(s_tag_data) == 0x14);

struct s_tag_block
{
	long count;
	union
	{
		void* address;
		byte* base;
	};
	s_tag_block_definition* definition;
};
static_assert(sizeof(s_tag_block) == 0xC);

template<typename t_element_type, dword ...t_extra>
//using c_typed_tag_block = s_tag_block;
struct c_typed_tag_block : s_tag_block
{
public:
	long count() const
	{
		return s_tag_block::count;
	}

	t_element_type* begin()
	{
		return static_cast<t_element_type*>(address);
	}

	t_element_type const* begin() const
	{
		return static_cast<t_element_type*>(address);
	}

	t_element_type* end()
	{
		return static_cast<t_element_type*>(address) + s_tag_block::count;
	}

	t_element_type const* end() const
	{
		return static_cast<t_element_type*>(address) + s_tag_block::count;
	}

	t_element_type& operator[](long index)
	{
		ASSERT(VALID_INDEX(index, s_tag_block::count));

		return static_cast<t_element_type*>(address)[index];
	}

	t_element_type& operator[](long index) const
	{
		ASSERT(VALID_INDEX(index, s_tag_block::count));

		return static_cast<t_element_type*>(address)[index];
	}

	void clear()
	{
		csmemset(address, 0, sizeof(t_element_type) * s_tag_block::count);
		s_tag_block::count = 0;
	}
};

struct s_tag_block_stub
{

};