#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/game/game_time.hpp

// mapped using halo 3/odst/halo reach
// only `_game_time_pause_reason_debug` works in multiplayer
enum e_game_time_pause_reason
{
	_game_time_pause_recursion_lock_internal = 0,
	_game_time_pause_debug,
	//_game_time_pause_debug_menu,
	_game_time_pause_ui,
	_game_time_pause_controller0_removal,
	_game_time_pause_controller1_removal,
	_game_time_pause_controller2_removal,
	_game_time_pause_controller3_removal,
	_game_time_pause_xbox_guide_ui,
	_game_time_pause_postgame,

	k_game_time_pause_reason_count
};

enum e_game_tick_publishing_flags
{
	_game_published_new_game_tick = 0,
	_game_published_shell_paused,
	_game_published_game_time_unchanged,
	_game_published_game_time_paused,
	_game_published_game_paused,
	_game_published_pregame,
	_game_published_main_time_halted,
	_game_published_game_speed_slowed,
	_game_published_framerate_infinite,
	_game_published_ui_request,
	_game_published_network_playback_client,
	_game_published_maintain_minimal_framerate,

	k_game_tick_publishing_flag_count
};

struct s_game_tick_time_samples
{
	void initialize()
	{
		reset();
	}

	void reset()
	{
		flags.clear();
		shell_dt = 0.0f;
		world_dt = 0.0f;
		game_dt = 0.0f;
		elapsed_game_ticks = 0;
	}

	void accum(s_game_tick_time_samples const* samples)
	{
		flags = samples->flags;
		shell_dt += samples->shell_dt;
		world_dt += samples->world_dt;
		game_dt += samples->game_dt;
		elapsed_game_ticks = samples->elapsed_game_ticks;
	}

	c_flags<e_game_tick_publishing_flags, dword, k_game_tick_publishing_flag_count> flags;
	real shell_dt;
	real world_dt;
	real game_dt;
	long elapsed_game_ticks;
};
static_assert(sizeof(s_game_tick_time_samples) == 0x14);

struct game_time_globals_definition
{
	bool initialized;
	c_flags<e_game_time_pause_reason, word, k_game_time_pause_reason_count> flags;
	short tick_rate;
	real tick_length;
	long time;
	real speed;
	real leftover_ticks;
	real rate_scale_timer;
	real rate_scale_duration;
	real rate_scale_initial;
	real rate_scale_final;
	long game_message_tick;
};
static_assert(sizeof(game_time_globals_definition) == 0x2C);

long game_time_get();
real game_ticks_to_seconds(long ticks);
long game_ticks_to_seconds_ceil(long ticks);
long game_seconds_integer_to_ticks(long seconds);
real game_seconds_to_ticks_real(real seconds);
real game_tick_length();
long game_tick_rate();
long game_seconds_to_ticks_round(real seconds);