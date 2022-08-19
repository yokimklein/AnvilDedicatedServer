#pragma once
#include "ring_buffer.h"

class c_sliding_window
{
	bool m_initialized;
	int m_size;
	int m_leading_sequence_number;
	int m_trailing_sequence_number;
	c_ring_buffer m_window;
};
static_assert(sizeof(c_sliding_window) == 0x1C);