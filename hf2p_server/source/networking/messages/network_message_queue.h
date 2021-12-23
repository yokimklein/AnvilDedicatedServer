#pragma once
#include <windows.h>

class c_network_message_queue
{
	byte data[0x64];
};
static_assert(sizeof(c_network_message_queue) == 0x64);