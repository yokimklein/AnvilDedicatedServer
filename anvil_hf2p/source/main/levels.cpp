#include "levels.h"

bool __fastcall levels_add_campaign(s_file_reference* file_reference)
{
	return INVOKE(0xDE220, levels_add_campaign, file_reference);
}

ulong __cdecl levels_get_available_map_mask()
{
	return INVOKE(0xDD750, levels_get_available_map_mask);
}