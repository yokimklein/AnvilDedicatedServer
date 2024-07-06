#pragma once
#include <memory\data.h>
#include <objects\objects.h>

struct s_crate_data : s_object_data
{
	long self_destruct; // timer?
};
static_assert(sizeof(s_crate_data) == 0x180);

bool crate_will_self_destruct(datum_index crate_index);