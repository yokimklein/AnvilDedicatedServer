#pragma once
#include <cseries\cseries.h>

enum e_game_state_revert_bit
{
	_game_state_revert_bit_user = 0,
	_game_state_revert_bit_scripting,
	_game_state_revert_bit_scripting_cinematic,

	k_game_state_revert_bits
};

enum e_main_pregame_frame
{
	_main_pregame_frame_none = 0,
	_main_pregame_frame_normal,
	_main_pregame_frame_loading_debug,
	_main_pregame_frame_cache_loading,
	_main_pregame_frame_crash_uploading,
	_main_pregame_frame_crash_done,
	_main_pregame_frame_upload,
	_main_pregame_frame_notify_out_of_sync,
	_main_pregame_frame_loading_screen,

	k_main_pregame_frame_count
};

struct s_scenario_zone_activation
{
	void clear()
	{
		deactivating_designer_zone_mask = 0;
		activating_designer_zone_mask = 0;
		deactivating_cinematic_zone_mask = 0;
		activating_cinematic_zone_mask = 0;
	}

	bool is_empty() const
	{
		return !deactivating_designer_zone_mask && !activating_designer_zone_mask && !deactivating_cinematic_zone_mask && !activating_cinematic_zone_mask;
	}

	long deactivating_designer_zone_mask;
	long activating_designer_zone_mask;
	long deactivating_cinematic_zone_mask;
	long activating_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_activation) == 0x10);

struct _main_globals
{
	bool suppress_startup_sequence;
	bool has_performed_startup_sequence;
	bool halted_with_errors;
	bool run_demos;
	bool exit_game;

	bool save_core;
	bool load_core;
	c_static_string<64> core_name;

	bool map_reset;
	bool map_reset_random;

	bool map_revert;
	c_flags<e_game_state_revert_bit, byte, k_game_state_revert_bits> map_revert_flags;

	bool save;
	bool save_and_exit;
	bool skip_cinematic;
	bool prepare_to_switch_zone_set;
	bool volatile switch_zone_set;
	bool user_interface_save_files;
	bool non_bsp_zone_activation;

	//bool cheat_drop_tag;
	byte __pad52[0x1]; // is this the above?

	bool game_state_decompression;
	bool reset_zone_resources;
	bool reloading_active_zone_set;
	bool activate_cinematic_tag;
	bool activate_cinematic_zone_from_tag;
	bool deactivate_cinematic_zone_from_tag;
	//bool cleanup_loading_screen; // unused is this the still here?
	long prepare_to_switch_zone_set_index;
	long switch_zone_set_index;
	s_scenario_zone_activation scenario_zone_activation;

	//dword cinematic_zone_group_tag;
	//long cinematic_zone_tag_index;
	byte __pad74[8]; // is this the above?

	long main_loop_pregame_entered;
	long main_loop_time;
};
static_assert(sizeof(_main_globals) == 0x84);

struct s_main_status_value
{
	char status_type[0x100];
	char status_data[0x200];
};
static_assert(sizeof(s_main_status_value) == 0x300);

extern s_main_status_value(&g_status_values)[32];
extern _main_globals& main_globals;
extern bool g_force_upload_even_if_untracked;

class c_tag_resources_game_lock
{
public:
	c_tag_resources_game_lock();
	~c_tag_resources_game_lock();

protected:
	long m_resource_key;
};
#define TAG_RESOURCES_GAME_LOCK(...) c_tag_resources_game_lock __local_tag_resource_scope_lock{}

class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(const char* file, long line);
	~c_wait_for_render_thread();

protected:
	ulong m_flags;
};
#define RENDER_THREAD_LOCK c_wait_for_render_thread STRCONCAT(__render_thread_lock, __LINE__)(__FILE__, __LINE__)

struct s_file_reference;

bool main_is_in_main_loop_pregame();
long __cdecl _internal_halt_render_thread_and_lock_resources(const char* file_name, long line_number);
void __fastcall main_exit_game();
void main_loop_pregame_disable(bool disable);
bool main_time_halted();
void main_write_stack_to_crash_info_status_file(const char* crash_info, void* context);
void main_status_dump(s_file_reference* file);
void main_halt_and_catch_fire();
void main_loop_pregame_halt_and_catch_fire_pop();
void main_loop_pregame_halt_and_catch_fire_push();
void __fastcall unlock_resources_and_resume_render_thread(ulong flags);

template<typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
bool main_status(const char* status_type, const char* format, parameters_t... parameters)
{
	return DECLFUNC(0x96D20, bool, __cdecl, const char*, const char*, ...)(status_type, format, parameters...);
}