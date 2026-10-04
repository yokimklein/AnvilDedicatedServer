#include "levels.h"

bool __fastcall levels_add_campaign(s_file_reference* file_reference)
{
	return INVOKE(ADDRESS_LEVELS_ADD_CAMPAIGN, levels_add_campaign, file_reference);
}

ulong __cdecl levels_get_available_map_mask()
{
	return INVOKE(ADDRESS_LEVELS_GET_AVAILABLE_MAP_MASK, levels_get_available_map_mask);
}