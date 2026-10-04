#include "online_lsp.h"
#include <cseries\cseries.h>

REFERENCE_DECLARE(ADDRESS_G_ONLINE_LSP_MANAGER, c_online_lsp_manager, g_online_lsp_manager);

void c_online_lsp_manager::go_into_crash_mode()
{
	INVOKE_CLASS_MEMBER(ADDRESS_C_ONLINE_LSP_MANAGER_GO_INTO_CRASH_MODE, c_online_lsp_manager, go_into_crash_mode);
}

c_online_lsp_manager* c_online_lsp_manager::get()
{
	return &g_online_lsp_manager;
}