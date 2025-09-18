#pragma once

class c_online_lsp_manager
{
public:
	void go_into_crash_mode();
	static c_online_lsp_manager* get();
};

extern c_online_lsp_manager& g_online_lsp_manager;