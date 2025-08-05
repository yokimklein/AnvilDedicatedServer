#pragma once

class c_animation_id
{
public:
	short m_subgraph;
	short m_index;
};
static_assert(sizeof(c_animation_id) == 0x4);