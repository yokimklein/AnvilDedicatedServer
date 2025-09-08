#include "restricted_memory_regions.h"
#include "cseries\cseries.h"
#include "memory\tls.h"
#include "cache\restricted_memory.h"

bool restricted_region_locked_for_current_thread(long index)
{
	TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	ASSERT(g_restricted_regions[index].initialized());
	return g_restricted_address[index] != NULL;
}

long restricted_region_add_member(long index, const char* name, const char* type, unsigned int allocation, long alignment_bits, void(__cdecl* tls_update_callback)(void*), void(__cdecl* tls_pre_overwrite_fixup_callback)(void*), void(__cdecl* tls_post_copy_fixup_callback)(void*))
{
	ASSERT(allocation);
	ASSERT(tls_update_callback);
	ASSERT(alignment_bits >= 0 && alignment_bits < 6);
	ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	long member_index = g_restricted_regions[index].add_member(name, type, allocation, alignment_bits, tls_update_callback, tls_pre_overwrite_fixup_callback, tls_post_copy_fixup_callback);
	ASSERT(member_index != NONE);
	void* address = restricted_region_get_member_address(index, member_index);
	ASSERT(align_pointer(address, alignment_bits) == address);
	tls_update_callback(address);
	return member_index;
}

void* restricted_region_get_member_address(long index, long member_index)
{
	ASSERT(index >= 0 && index < k_total_restricted_memory_regions);
	TLS_DATA_GET_VALUE_REFERENCE(g_restricted_address);
	void* address = g_restricted_address[index];
	ASSERT(address != NULL);
	return (byte*)address + g_restricted_regions[index].get_member_offset(member_index);
}
