#pragma once
#include <cstdint>
#include <windows.h>

#pragma pack(push, 4)
class c_bitstream
{
	byte* m_data;
	byte* m_data_max;
	int32_t m_data_size_bytes;
	int32_t m_data_size_alignment;
	uint32_t m_state;
	void* unknown14;
	int32_t m_data_size_in_bits;
	int32_t m_position;
	uint64_t m_window;
	int32_t m_window_bits_used;
	c_bitstream* m_next_data;
	uint32_t unknown30;
	uint32_t unknown34;
	uint32_t unknown38;
	uint32_t unknown3C;
	uint32_t unknown40;
	void* unknown44;
	void* unknown48;
	byte unknown4C[76];
	uint32_t unknown98;
	uint32_t unknown9C;
};
static_assert(sizeof(c_bitstream) == 0xA0);
#pragma pack(pop)
