#pragma once
#include "..\cseries\cseries.h"

class c_aim_target_object
{
	uchar type;
	long model_target_index;
	long object_index;
};
static_assert(sizeof(c_aim_target_object) == 0xC);