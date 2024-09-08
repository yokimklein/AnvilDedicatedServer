#pragma once
#include <camera\director.h>

struct c_editor_director : public c_director
{
	long __unknown168;
	bool m_scripted;
	bool __unknown16D;

	byte pad[0x2];
	byte unused[0xC];

	//static void __cdecl bump_speed();
	//static real __cdecl get_speed();
	//void __cdecl set_position_and_roll(real_point3d const* position, euler_angles2d const* facing);
	//void __cdecl set_scripted(bool scripted);
};
static_assert(sizeof(c_editor_director) == 0x160);