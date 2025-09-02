#pragma once
#include <cseries/cseries.h>
#include <animations/animation_data.h>

struct c_channel_base
{
	long m_graph_index;
	long m_impulse_name;
	c_animation_id m_animation_id;
	word_flags m_playback_flags;
	char m_graph_location;
	char m_parent_slider_index;
	byte m_parent_slider_type;
	char m_initialized;
};
static_assert(sizeof(c_channel_base) == 0x14);