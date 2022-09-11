#pragma once
#include "..\cseries\cseries.h"
#include "..\memory\bitstream.h"

#pragma pack(push, 1)
class c_network_time_statistics
{
	ulong unknown1;
	ulong unknown2;
	ulong unknown3;
	ulong unknown4;
	ulong unknown5;
	long64 unknown6;
	ulong m_period_duration_msec;
	ulong m_interval_duration_msec;
	float unknown9;
	ulong m_next_interval_index;
	c_bitstream stream; // is this accurate?
	long64 unknown11;
	ulong unknown12;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);
#pragma pack(pop)

class c_network_window_statistics
{
	byte data[0x110];
};
static_assert(sizeof(c_network_window_statistics) == 0x110);