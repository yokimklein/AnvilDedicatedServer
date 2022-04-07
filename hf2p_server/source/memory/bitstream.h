#pragma once
#include <cstdint>
#include <windows.h>

class c_bitstream
{
	byte* start;
	byte* end;
	uint32_t size;
	uint32_t unknownC;
	uint32_t unknown10;
	void* unknown14;
	uint32_t unknown18;
	uint32_t position;
	uint64_t window;
	long windowBitsUsed;
	c_bitstream* currentPtr;
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
