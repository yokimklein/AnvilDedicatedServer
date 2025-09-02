#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

struct invitation_data
{
	short behavior_index;
	short slot;
	long joint_type;
	long jindex;
	long expiration_time;
};
static_assert(sizeof(invitation_data) == 0x10);
