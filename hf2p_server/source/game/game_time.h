#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/game/game_time.hpp

// mapped using halo 3/odst/halo reach
// only `_game_time_pause_reason_debug` works in multiplayer
enum e_game_time_pause_reason
{
	// set/unset at `game_time_update` begin/end
	_game_time_pause_reason_unknown0 = 0,

	// game_time_update
	// - debug_pause_game != debug_pause_game_active
	_game_time_pause_reason_debug,

	// halo 3: c_start_menu_game_campaign::initialize
	// odst: c_start_menu_screen_widget::update
	// halo reach: c_start_menu_pause_component::update_pause
	// game_state_call_after_load_procs
	//	- game_state_set_revert_time
	_game_time_pause_reason_ui,

	// game_time_update
	// - controllers
	_game_time_pause_reason_controller0,
	_game_time_pause_reason_controller1,
	_game_time_pause_reason_controller2,
	_game_time_pause_reason_controller3,

	// user_interface_xbox_guide_is_active
	_game_time_pause_reason_xbox_guide,

	// metagame: load postgame carnage report
	_game_time_pause_reason_postgame,

	k_game_time_pause_reason_count,
};

struct s_game_tick_time_samples
{
	long flags;
	real __unknown4;
	real __unknown8;
	real __unknownC;
	dword __unknown10;
};
static_assert(sizeof(s_game_tick_time_samples) == 0x14);

struct s_game_time_globals
{
	bool initialized;
	c_flags<e_game_time_pause_reason, short, k_game_time_pause_reason_count> flags;
	short tick_rate;
	word : 16;
	real seconds_per_tick;
	long elapsed_ticks;
	real speed;
	real __unknown14;
	s_game_tick_time_samples time_samples;
};
static_assert(sizeof(s_game_time_globals) == 0x2C);

long game_time_get();
real game_ticks_to_seconds(long ticks);
long game_ticks_to_seconds_ceil(long ticks);
long game_seconds_integer_to_ticks(long seconds);
real game_seconds_to_ticks_real(real seconds);