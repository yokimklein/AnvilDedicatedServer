#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/integer_math.hpp

union short_bounds
{
	struct
	{
		short lower;
		short upper;
	};
	short n[2];
};
static_assert(sizeof(short_bounds) == 0x4);

union long_bounds
{
	struct
	{
		long lower;
		long upper;
	};
	long n[2];
};
static_assert(sizeof(long_bounds) == 0x8);

union long64_bounds
{
	struct
	{
		long64 lower;
		long64 upper;
	};
	long64 n[2];
};
static_assert(sizeof(long64_bounds) == 0x10);

union point2d
{
	struct
	{
		short x;
		short y;
	};
	short n[2];
};
static_assert(sizeof(point2d) == 0x4);

union point3d
{
	struct
	{
		short x;
		short y;
		short z;
	};
	short n[3];
};
static_assert(sizeof(point3d) == 0x6);

union long_point2d
{
	struct
	{
		long x;
		long y;
	};
	long n[2];
};
static_assert(sizeof(long_point2d) == 0x8);

union long_point3d
{
	struct
	{
		long x;
		long y;
		long z;
	};
	long n[3];
};
static_assert(sizeof(long_point3d) == 0xC);

union rectangle2d
{
	struct
	{
		short y0;
		short x0;
		short y1;
		short x1;
	};
	short n[4];
};
static_assert(sizeof(rectangle2d) == 0x8);

union long_rectangle2d
{
	struct
	{
		long y0;
		long x0;
		long y1;
		long x1;
	};
	long n[4];
};
static_assert(sizeof(long_rectangle2d) == 0x10);

union rgb_color
{
	struct
	{
		byte : 8;
		byte red;
		byte green;
		byte blue;
	};
	byte n[4];
	ulong value;
};
static_assert(sizeof(rgb_color) == 0x4);

union argb_color
{
	struct
	{
		byte alpha;
		byte red;
		byte green;
		byte blue;
	};
	byte n[4];
	ulong value;
};
static_assert(sizeof(argb_color) == 0x4);