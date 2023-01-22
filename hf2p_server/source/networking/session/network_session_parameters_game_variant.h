#pragma once
#include "network_session_parameters_chunked.h"
#include "..\..\game\game_variant.h"

struct s_network_session_parameter_game_variant
{
	bool valid;
	c_game_variant game_variant;
};
static_assert(sizeof(s_network_session_parameter_game_variant) == 0x268);

class c_network_session_parameter_game_variant : public c_network_session_parameter_chunked<s_network_session_parameter_game_variant, 0x1800>
{
public:
	c_game_variant* get();
};
static_assert(sizeof(c_network_session_parameter_game_variant) == 0x3570);