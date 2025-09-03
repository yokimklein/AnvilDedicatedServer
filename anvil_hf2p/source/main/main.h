#pragma once
#include <cseries\cseries.h>

enum e_game_state_revert_bit
{
	_game_state_revert_bit_user = 0,
	_game_state_revert_bit_scripting,
	_game_state_revert_bit_scripting_cinematic,

	k_game_state_revert_bits
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

extern _main_globals& main_globals;

bool main_is_in_main_loop_pregame();
long __cdecl internal_halt_render_thread_and_lock_resources(const char* file_name, long line_number);
void __fastcall main_exit_game();