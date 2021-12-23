#pragma once
#include <windows.h>

class c_network_connection
{
	byte unknown_data[0x960];
};
static_assert(sizeof(c_network_connection) == 0x960);