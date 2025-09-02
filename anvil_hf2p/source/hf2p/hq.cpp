#include "hq.h"

c_static_string<128> g_tutorial_scenario_path = "levels\\solo\\s3d_tutorial\\s3d_tutorial";
c_static_wchar_string<128> g_tutorial_map_name = L"s3d_tutorial";

void __cdecl hq_start_tutorial_level()
{
	INVOKE(0x33AAC0, hq_start_tutorial_level);
}