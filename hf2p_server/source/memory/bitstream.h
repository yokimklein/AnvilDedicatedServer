#pragma once
#include <cseries\cseries.h>

#pragma pack(push, 4)
class c_bitstream
{
	byte* m_data;
	byte* m_data_max;
	long m_data_size_bytes;
	long m_data_size_alignment;
	ulong m_state;
	void* unknown14;
	long m_data_size_in_bits;
	long m_position;
	qword m_window;
	long m_window_bits_used;
	c_bitstream* m_next_data;
	ulong unknown30;
	ulong unknown34;
	ulong unknown38;
	ulong unknown3C;
	ulong unknown40;
	void* unknown44;
	void* unknown48;
	byte unknown4C[76];
	ulong unknown98;
	ulong unknown9C;
};
static_assert(sizeof(c_bitstream) == 0xA0);
#pragma pack(pop)
