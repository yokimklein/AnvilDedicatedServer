#pragma once
#include <cseries\cseries.h>
#include <memory\bitstream.h>

#pragma pack(push, 1)
class c_network_time_statistics
{
public:
	ulong unknown1;
	ulong unknown2;
	ulong unknown3;
	ulong unknown4;
	ulong unknown5;
	long64 unknown6;
	ulong m_period_duration_msec;
	ulong m_interval_duration_msec;
	real m_unknown_period; // set to 1000.0f / m_period_duration_msec
	ulong m_next_interval_index;
	byte __data2C[0x8C];
	bool bandwidth_unstable;
	byte __data41[0x13];
	qword __unknownCC;
	byte __dataD4[0x04];
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);
#pragma pack(pop)

class c_network_window_statistics
{
	byte data[0x110];
};
static_assert(sizeof(c_network_window_statistics) == 0x110);