#pragma once
#include <windows.h>

class c_network_time_statistics
{
	byte data[0xD8];
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);

class c_network_window_statistics
{
	byte data[0x110];
};
static_assert(sizeof(c_network_window_statistics) == 0x110);