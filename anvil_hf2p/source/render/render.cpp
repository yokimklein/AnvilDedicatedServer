#include "render.h"

REFERENCE_DECLARE(ADDRESS_C_RENDER_GLOBALS_M_FRAME_INDEX, ulong, c_render_globals::m_frame_index);

long c_render_globals::get_frame_index()
{
	return m_frame_index;
}
