#include "restricted_memory.h"

//REFERENCE_DECLARE_ARRAY(0x0, const char*, g_restricted_region_names, k_total_restricted_memory_regions);
REFERENCE_DECLARE_ARRAY(0x3F16310, c_restricted_section, g_restricted_section, k_total_restricted_memory_regions);
REFERENCE_DECLARE_ARRAY(0x3FDCAB8, c_restricted_memory, g_restricted_regions, k_total_restricted_memory_regions);

bool c_restricted_memory::initialized()
{
	return TEST_BIT(m_internal_flags, 0);
}

ulong c_restricted_memory::add_member(const char* name, const char* type_name, long original_requested_size, long alignment_bits, void(__cdecl* update_function)(void*), void(__cdecl* overwrite_function)(void*), void(__cdecl* post_copy_function)(void*))
{
	return INVOKE_CLASS_MEMBER(0x1690E0, c_restricted_memory, add_member, name, type_name, original_requested_size, alignment_bits, update_function, overwrite_function, post_copy_function);
}

ulong c_restricted_memory::get_member_offset(ulong member_index)
{
	ASSERT(initialized());
	ASSERT(initialized()); // seems to have been typo'd and duplicated by accident in source?
	ASSERT(member_index >= 0 && member_index < m_registered_member_count);
	ASSERT(m_registered_member[member_index].size > 0);
	return m_registered_member[member_index].member_offset;
}
