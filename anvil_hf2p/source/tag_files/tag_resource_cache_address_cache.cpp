#include "tag_resource_cache_address_cache.h"
#include "memory\data.h"

void c_tag_resource_address_cache_control_interface::staleify_resource_address(datum_index index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);

	//tag_resource_cache_handle_get_external_handle(index);
	BIT_VECTOR_AND_FLAG(m_address_cache->m_ready_resource_addresses.m_bit_storage, absolute_index);
	m_address_cache->m_cached_resource_snapshots[absolute_index] = NONE;
}

void c_tag_resource_address_cache_control_interface::mark_required_resource(datum_index index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
	BIT_VECTOR_OR_FLAG(m_address_cache->m_required_resource_addresses.m_bit_storage, absolute_index);
}

void c_tag_resource_address_cache_control_interface::mark_unrequired_resource(datum_index index)
{
	long absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);
	BIT_VECTOR_AND_FLAG(m_address_cache->m_required_resource_addresses.m_bit_storage, absolute_index);
}
