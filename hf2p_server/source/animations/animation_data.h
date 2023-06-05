#pragma once

class c_animation_id
{
public:
	short graph_index;
	short animation_index;
};
static_assert(sizeof(c_animation_id) == 0x4);