#pragma once
#include "..\objects\objects.h"

struct s_item_data : s_object_data
{
	// s_item_data is 4 bytes longer in ms29 vs ms23
	// an extra long was added somewhere before unknown_state - TODO: FINISH RESEARCHING THIS
	long unknown;

	char item_flags;
	char unknown_state;
	char : 8;
	char : 8;
	datum_index ignore_object_index; // offset is 0x4 greater in ms29, something was added before this
	long last_relevant_time;
	datum_index owner_unit_index;
	long : 32;
};
static_assert(sizeof(s_item_data) == 0x190);