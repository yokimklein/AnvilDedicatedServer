#include "tag_groups.h"
#include "cache\cache_files.h"
#include <stdlib.h>
#include <string.h>

void* s_tag_reference::get_definition()
{
	if (index == NONE)
	{
		return NULL;
	}

	return tag_get(group_tag, index);
}

const char* s_tag_reference::get_name()
{
	if (name)
	{
		return name;
	}

	// $TODO: !!!

	//if (!VALID_INDEX(index, g_cache_file_globals.header.debug_tag_name_count))
	//{
	//	return "<unknown>";
	//}
	//
	//if (const char* _name = tag_get_name_safe(index))
	//{
	//	name = _name;
	//	name_length = csstrnlen(_name, _MAX_PATH);
	//
	//	//c_console::write_line("setting initial tag name: '%s.%s'", name, get_group_name());
	//
	//	return _name;
	//}

	return "<unknown>";
}

const char* s_tag_reference::get_group_name()
{
	// $TODO: !!!

	//if (group_tag != NONE)
	//{
	//	return tag_group_get_name(group_tag);
	//}
	//
	//if (!g_cache_file_globals.tag_instances || !g_cache_file_globals.tag_index_absolute_mapping)
	//{
		return "<unknown>";
	//}
	//
	//return g_cache_file_globals.tag_instances[g_cache_file_globals.tag_index_absolute_mapping[index]]->tag_group.name.get_string();
}


void* tag_block_get_element_with_size(s_tag_block const* block, long index, long size)
{
	ASSERT(block);
	ASSERT(block->count >= 0);
	ASSERT(VALID_INDEX(index, block->count));
	ASSERT(block->address);

	return block->base + index * size;
}

const char* tag_name_strip_path(const char* path)
{
	const char* name = strrchr(path, '\\');
	if (name)
	{
		return name + 1;
	}
	return path;
}

const wchar_t* tag_name_strip_path(const wchar_t* path)
{
	const wchar_t* name = wcsrchr(path, '\\');
	if (name)
	{
		return name + 1;
	}
	return path;
}

void tag_block_set_elements(s_tag_block* block, void* elements)
{
	block->address = elements;
}

void tag_block_set_element_count(s_tag_block* block, long count)
{
	block->count = count;
}
