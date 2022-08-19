#pragma once
#include <windows.h>
#include <stdint.h>
#include "..\memory\bitstream.h"

#pragma pack(push, 4)
class c_network_time_statistics
{
	uint32_t unknown1;
	uint32_t unknown2;
	uint32_t unknown3;
	uint32_t unknown4;
	uint32_t unknown5;
	uint64_t unknown6;
	uint32_t unknown7;
	float unknown8;
	float unknown9;
	uint32_t unknown10;
	c_bitstream stream;
	uint64_t unknown11;
	uint32_t unknown12;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);
#pragma pack(pop)

class c_network_window_statistics
{
	byte data[0x110];
};
static_assert(sizeof(c_network_window_statistics) == 0x110);