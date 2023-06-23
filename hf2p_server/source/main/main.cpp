#include "main.h"
#include <cseries\cseries.h>

bool main_is_in_main_loop_pregame()
{
	long* value = (long*)base_address(0x3EE1EC4);
	return *value != 0;
}