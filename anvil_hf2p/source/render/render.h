#pragma once
#include "cseries\cseries.h"

class c_render_globals
{
public:
	//static void __cdecl advance_frame_time(real seconds_elapsed);

	//static bool __cdecl get_depth_fade_active();
	//static bool __cdecl get_distortion_active();
	//static bool __cdecl get_distortion_history();
	//static bool __cdecl get_distortion_visible();
	static long get_frame_index();
	//static real __cdecl get_frame_time();

	//static void __cdecl increment_frame_index();

	//static void __cdecl set_depth_fade_active(bool depth_fade_active);
	//static void __cdecl set_distortion_active(bool distortion_active);
	//static void __cdecl set_distortion_history(bool distortion_history);
	//static void __cdecl set_distortion_visible(bool distortion_visible);
	//static void __cdecl set_frame_index(long frame_index);
	//static void __cdecl set_frame_time(real frame_time);

	//static void __cdecl set_weather_occlusion_available(bool weather_occlusion_available);
	//static bool __cdecl get_weather_occlusion_available();

private:
	static ulong& m_frame_index;
	//static real& m_frame_time;
	//static bool& m_distortion_active;
	//static bool& m_distortion_visible;
	//static bool& m_distortion_history;
	//static bool& m_depth_fade_active;
	//static bool& m_weather_occlusion_available;
	//static bool& m_render_pc_albedo_lighting;
};