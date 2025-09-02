#pragma once
#include <networking\logic\life_cycle\life_cycle_state_handler.h>

enum e_end_game_write_stats_flags
{
	_end_game_write_stats_session_start_initiated_bit = 0,
	_end_game_write_stats_session_start_completed_bit,

	// removed since ms23
	//_end_game_write_stats_write_initiated_bit,
	//_end_game_write_stats_write_completed_bit,
	//_end_game_write_stats_write_blocked_bit,

	_end_game_write_stats_session_end_initiated_bit,
	_end_game_write_stats_session_end_completed_bit,
	_end_game_write_stats_data_mine_upload_initiated_bit,
	_end_game_write_stats_data_mine_upload_completed_bit,
	_end_game_write_stats_webstats_submitted_bit,
	_end_game_write_stats_initiated_bit,
	_end_game_write_stats_suppressed_bit,
	_end_game_write_stats_bypassed_bit,
	_end_game_write_stats_completed_bit,

	// these flags originally came after start_completed and were removed since ms23, but I've added them back here to use to submit stats
	_end_game_write_stats_write_initiated_bit,
	_end_game_write_stats_write_completed_bit,

	k_end_game_write_stats_flags_count
};

enum e_end_game_stats_write_desire
{
	_end_game_stats_write_desire_no_reason = 0,
	_end_game_stats_write_desire_not_live,
	_end_game_stats_write_desire_game_options_not_valid,
	_end_game_stats_write_desire_game_is_campaign,
	_end_game_stats_write_desire_game_is_film,
	_end_game_stats_write_desire_results_not_finalized,
	_end_game_stats_write_desire_cant_get_final_stats,
	_end_game_stats_write_desire_stats_uninitialized,
	_end_game_stats_write_desire_stats_unreliable,
	_end_game_stats_write_desire_simulation_aborted,
	_end_game_stats_write_desire_not_enough_machines,
	_end_game_stats_write_desire_write_desired,

	k_end_game_stats_write_desire_count,
};


class c_life_cycle_state_handler_end_game_write_stats : public c_life_cycle_state_handler
{
public:
	void update_()/* override*/; // $TODO: replace in original virtual table

	void update_write_stats();
	e_end_game_stats_write_desire game_get_stats_write_desire() const;
	void update_session_start();
	void update_session_end();
	void update_submit_webstats();
	void update_data_mine_upload();

protected:
	ulong m_end_game_write_stats_start_time;
	c_flags<e_end_game_write_stats_flags, word, k_end_game_write_stats_flags_count> m_flags;
};
static_assert(sizeof(c_life_cycle_state_handler_end_game_write_stats) == 0x18);