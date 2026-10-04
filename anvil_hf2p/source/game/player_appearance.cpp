#include "player_appearance.h"

const char* __fastcall modifier_get_name(e_modifiers modifier)
{
	ASSERT(VALID_INDEX(modifier, k_modifiers_count));
	return INVOKE(ADDRESS_MODIFIER_GET_NAME, modifier_get_name, modifier);
}