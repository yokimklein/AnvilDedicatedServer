#pragma once
#include <camera\director.h>

struct c_debug_director : public c_director
{
	//void constructor(long user_index);

	void update_(real dt);
	void changed_camera();
	void cycle_camera();
	bool finished_cycle();

protected:
	long m_current_camera_mode_index;
	bool m_input_inhibited;
	bool m_flying_camera_biped_control;

	byte pad[0x2];
	byte unused[0xC];
};
static_assert(sizeof(c_debug_director) == 0x160);