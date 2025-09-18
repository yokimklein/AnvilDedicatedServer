#include "online_lsp.h"
#include <cseries\cseries.h>

REFERENCE_DECLARE(0x394B5C0, c_online_lsp_manager, g_online_lsp_manager);

void c_online_lsp_manager::go_into_crash_mode()
{
	INVOKE_CLASS_MEMBER(0x5E80, c_online_lsp_manager, go_into_crash_mode);
}

c_online_lsp_manager* c_online_lsp_manager::get()
{
	return &g_online_lsp_manager;
}