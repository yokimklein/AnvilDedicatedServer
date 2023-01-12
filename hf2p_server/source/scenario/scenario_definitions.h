#pragma once
#include "..\cseries\cseries.h"

struct s_scenario_location
{
	s_cluster_reference cluster_reference;
};
static_assert(sizeof(s_scenario_location) == 0x2);