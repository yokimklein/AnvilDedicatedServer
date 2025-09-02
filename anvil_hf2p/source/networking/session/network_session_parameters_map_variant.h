#pragma once
#include <networking\session\network_session_parameters_chunked.h>
#include <scenario\scenario_map_variant.h>

struct s_network_session_parameter_map_variant
{
	bool valid;
	long : 32; // padding
	c_map_variant map_variant;
};
static_assert(sizeof(s_network_session_parameter_map_variant) == 0xE098);

class c_network_session_parameter_map_variant : public c_network_session_parameter_chunked<s_network_session_parameter_map_variant, 0x7000>
{
};
static_assert(sizeof(c_network_session_parameter_map_variant) == 0x2A1D0);