#pragma once
#include "..\cseries\cseries.h"

class c_aim_target_object
{
	uchar type;
	long model_target_index;
	long object_index;
};
static_assert(sizeof(c_aim_target_object) == 0xC);

struct c_aim_target_predicted
{
	byte target_type;
	dword aim_target;
	dword target_model;
};
static_assert(sizeof(c_aim_target_predicted) == 0xC);