#pragma once
#include <windows.h>
#include "..\..\memory\sliding_window.h"

class c_network_message_queue
{
	public:
		bool has_channel_been_used();
	
		byte data1[0x7];
		bool m_channel_has_been_used; // non-original name
		byte data2[0xC];
		c_sliding_window m_sliding_window;
		byte data3[0x34];
};
static_assert(sizeof(c_network_message_queue) == 0x64);