#include "levels.h"

bool levels_add_campaign(s_file_reference* file_reference)
{
	return DECLFUNC(base_address(0xDE220), bool, __thiscall, s_file_reference*)(file_reference);
}