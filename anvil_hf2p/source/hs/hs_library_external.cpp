#include "hs_library_external.h"
#include "interface\terminal.h"
#include "cseries\cseries_events.h"

void __fastcall hs_print(const char* s)
{
	terminal_printf(global_real_argb_green, s);
}

void __fastcall hs_log_print(const char* s)
{
	event(_event_warning, "hs: %s", s);
}
