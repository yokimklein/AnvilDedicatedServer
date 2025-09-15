#include "render.h"

REFERENCE_DECLARE(0xEAC1AC, ulong, c_render_globals::m_frame_index);

long c_render_globals::get_frame_index()
{
	return m_frame_index;
}
