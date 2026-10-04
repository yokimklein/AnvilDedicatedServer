#include "main_time.h"
#include "cseries\cseries.h"

REFERENCE_DECLARE(ADDRESS_DISPLAY_FRAMERATE, bool, display_framerate);

void __fastcall main_time_frame_rate_display()
{
	INVOKE(ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY, main_time_frame_rate_display);
}
