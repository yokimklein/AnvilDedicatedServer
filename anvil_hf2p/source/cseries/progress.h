#pragma once

struct progress_callbacks
{
	void(__cdecl* progress_new_proc)(const char*, void*);
	void(__cdecl* progress_update_proc)(const char*, const char*, long, void*);
	void(__cdecl* progress_done_proc)(const char*, void*);
	void* progress_data;
};

// this struct is unnamed in the original code
struct s_progress_globals
{
	progress_callbacks default_callbacks;
	progress_callbacks custom_callbacks;
	bool disable_callbacks;
	ulong start_time;
	bool currently_valid;
	char description[256];
	long last_progress;
	ulong last_milliseconds;
};
static_assert(sizeof(s_progress_globals) == 0x134);

extern s_progress_globals& progress_globals;

void progress_set_default_callbacks(const progress_callbacks* callbacks);