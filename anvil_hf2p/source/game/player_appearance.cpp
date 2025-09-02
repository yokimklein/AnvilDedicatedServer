#include "player_appearance.h"

const char* __fastcall modifier_get_name(e_modifiers modifier)
{
	ASSERT(VALID_INDEX(modifier, k_modifiers_count));
	return INVOKE(0x2E77A0, modifier_get_name, modifier);
}