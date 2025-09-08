#include "visibility_collection.h"
#include "cache\restricted_memory_regions.h"

REFERENCE_DECLARE(0x40BB4A8, c_visibility_globals_keeper, g_visibility_globals_keeper);

c_visibility_collection* get_global_camera_collection()
{
	ASSERT(restricted_region_locked_for_current_thread(k_game_state_render_region));
	return &g_visibility_globals_keeper.m_visibility_globals.camera_visibility;
}

bool __vectorcall visibility_volume_test_sphere(visibility_volume* volume, const real_point3d* point, real radius)
{
	// $TODO: verify calling convention works
	return INVOKE(0x1CCE30, visibility_volume_test_sphere, volume, point, radius);
}
