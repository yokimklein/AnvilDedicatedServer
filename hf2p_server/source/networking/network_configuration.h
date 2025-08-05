#pragma once
#include <cseries\cseries.h>
#include <simulation\game_interface\simulation_game_events.h>
#include <simulation\game_interface\simulation_game_entities.h>
#include <networking\session\network_observer.h>

struct s_network_file_download_configuration
{
	long master_catalog_file_download_interval_msec;
	long required_file_invalidation_check_interval_msec;
	long required_file_download_retry_interval_msec;
};
static_assert(sizeof(s_network_file_download_configuration) == 0xC);

struct s_bandwidth_configuration_host_preference_table
{
	long connectivity_multiplier;
	long host_bonus;
	long desired_hostable_peers_multiplier;
	long maximum_hostable_peers_multiplier;
	long unbounded_hostable_peers_multiplier;
	long latency_table_multiplier;
	long gamestate_bonus;
	long open_nat_bonus;
	long hard_drive_bonus;
	long local_user_table_multiplier;
	long host_rating_multiplier;
	long connectivity_rating_multiplier;
	long synchronous_hostable_peers_multiplier;
};
static_assert(sizeof(s_bandwidth_configuration_host_preference_table) == 0x34);

// $TODO: finish mapping this
struct s_bandwidth_configuration
{
	real bandwidth_outlier_discard_fraction;
	long bandwidth_minimum_measurement_count;
	long bandwidth_safety_margin_bps;
	long bandwidth_minimum_known_good_bps;
	long bandwidth_tracking_minimum_duration_msec;
	real bandwidth_tracking_maximum_satiation;

	long bandwidth_dispute_minimum_count;
	long bandwidth_dispute_threshold_bps;
	long bandwidth_dispute_increase_bps;
	long host_speculative_migration_check_interval_msec;
	long host_speculative_migration_check_interval_custom_msec;
	long host_speculative_migration_check_interval_matchmaking_msec;
	long host_speculative_migration_remigrate_interval_msec;
	long host_speculative_migration_required_lobby_peer_connectivity_difference;
	long host_speculative_migration_required_match_host_rating_difference;
	long host_speculative_migration_required_match_host_bandwidth_difference;
	long host_speculative_migration_required_custom_host_rating_difference;
	long host_speculative_migration_required_custom_host_bandwidth_difference;
	byte host_preference_latency_table[20];
	byte host_preference_local_user_table[5];
	s_bandwidth_configuration_host_preference_table host_preferences[4];
	long host_preference_connectivity_rankings_spread;
	long host_preference_host_rankings_spread;
	long upstream_bandwidth_absolute_minimum_bps[18];
	long upstream_bandwidth_requirement_bps[18];
	long upstream_bandwidth_desired_bps[18];
	long minimum_player_restricted_count;
	long minimum_host_upstream_bandwidth_bps;
	long minimum_host_downstream_bandwidth_bps;
	long minimum_host_delegation_advantage_bps;
	long good_host_upstream_bandwidth_bps;
	long good_host_downstream_bandwidth_bps;
	long minimum_voice_repeater_upstream_bandwidth_bps;
	long minimum_voice_repeater_downstream_bandwidth_bps;
	long voice_channel_bandwidth_bps;
};
static_assert(sizeof(s_bandwidth_configuration) == 0x238);

struct s_session_configuration
{
	long session_recreate_timeout_msec;
	long join_request_interval_msec;
	long join_secure_connection_timeout_msec;
	long join_initial_update_timeout_msec;
	long join_time_to_hold_in_join_queue_msec;
	long join_notify_client_join_in_queue_interval_msec;
	long join_abort_interval_msec;
	long join_abort_timeout_msec;
	long host_rejoin_accept_timeout_msec;
	long leave_timeout_msec;
	long leave_request_interval_msec;
	long host_handoff_initiate_timeout_msec;
	long host_handoff_selection_delay_msec;
	long host_handoff_selection_timeout_msec;
	long host_transition_timeout_msec;
	long host_reestablish_timeout_msec;
	long host_reestablish_maximum_send_to_original_host_delay_msec;
	long election_failsafe_timeout_msec;
	long election_peer_timeout_msec;
	long election_ignore_dissension_msec;
	long guaranteed_election_send_interval_msec;
	long time_synchronization_interval_msec;
	long time_synchronization_retry_msec;
	long minimum_election_send_interval_msec;
	long allow_third_party_host_elections;
};
static_assert(sizeof(s_session_configuration) == 0x64);

struct s_observer_configuration
{
	long secure_connect_attempts;
	long secure_connect_intervals[8];
	long connect_attempts;
	long connect_intervals[8];
	long non_simulation_reconnect_attempts;
	long non_simulation_reconnect_intervals[8];
	long reconnect_attempts;
	long reconnect_intervals[8];
	long death_recovery_time;
	long heartbeat_send_timeout;
	long connection_active_send_timeout;
	long connection_alive_send_timeout;
	long connection_alive_receive_timeout;
	long connection_drop_minimum_active_time;
	long connection_drop_receive_timeout;
	long synchronous_connection_drop_minimum_active_time;
	long synchronous_connection_drop_receive_timeout;
	long minimum_undesired_connection_timeout_msec;
	long maximum_undesired_connection_timeout_msec;
	real safety_window_threshold;
	real safety_packet_interval;
	real safety_packet_maximum_interval;
	long packet_rate_multiplier_count;
	real packet_rate_multipliers[16];
	long packet_window_minimum_bytes;
	real voice_fraction_authority;
	real voice_fraction_client;
	real voice_fraction_non_simulation;
	real voice_maximum_packet_rate;
	long voice_receive_fast_acknowledge_time;
	long bandwidth_interval;
	long loss_detection_window_size;
	long flood_max_aperture_msec;
	real flood_packet_fraction;
	long desired_payload_bytes_out_of_game;
	long desired_payload_bytes_simulation;
	real desired_minimum_packet_rate_multiplier;
	long minimum_bandwidth_bps;
	long bandwidth_adjust_interval;
	real bandwidth_flood_channel_fraction;
	long bandwidth_flood_check_threshold;
	long bandwidth_flood_backoff_kbps;
	long bandwidth_flood_backoff_repeat_interval;
	long bandwidth_clear_check_threshold;
	long bandwidth_check_threshold_maximum;
	long bandwidth_initial_backoff_kbps;
	long bandwidth_known_good_minimum_kbps;
	long bandwidth_increment_known_bad_threshold_kbps;
	long bandwidth_increment_kbps;
	bool generate_stream_events;
	long observer_stream_expansion_interval_msec;
	long stream_minimum_bps;
	long stream_maximum_bps;
	long stream_initial_startup_msec;
	long stream_initial_bps;
	long stream_initial_total_bps_unreliable;
	long stream_initial_total_bps_reliable;
	long stream_initial_minimum_bps_per_stream;
	long stream_minimum_great_bandwidth_bps;
	long stream_out_of_game_upstream_bandwidth_bps;
	long stream_distributed_client_upstream_bandwidth_bps;
	long stream_distributed_host_upstream_bandwidth_bps;
	long stream_synchronous_client_initial_upstream_bandwidth_bps;
	long stream_synchronous_host_initial_upstream_bandwidth_bps;
	long stream_synchronous_joining_host_initial_upstream_bandwidth_bps;
	long stream_synchronous_client_upstream_bandwidth_bps;
	long stream_synchronous_host_upstream_bandwidth_bps;
	long stream_synchronous_joining_host_upstream_bandwidth_bps;
	long stream_synchronous_non_joining_host_upstream_bandwidth_bps;
	long stream_synchronous_client_minimum_upstream_bandwidth_bps;
	long stream_synchronous_host_minimum_upstream_bandwidth_bps;
	long stream_rtt_noise_msec;
	long stream_minimum_rtt_msec;
	long stream_rtt_average_gain_bits;
	long stream_loss_window_size;
	real stream_loss_throttle_fraction;
	long stream_throttle_rtt_multiplier;
	real stream_throttle_reduce_multiplier;
	long stream_throttle_cookie_event_count;
	long stream_throttle_cookie_backoff_threshold;
	long stream_bandwidth_step_bps;
	real stream_bandwidth_step_max_fraction;
	long stream_bandwidth_backoff_bps;
	real stream_bandwidth_backoff_max_fraction;
	long stream_period_maximum_msec;
	long stream_growth_period_minimum_msec;
	long stream_throughput_recording_period_minimum_msec;
	long stream_throughput_satiated_bandwidth_bps;
	real stream_throughput_satiated_stream_fraction;
	long stream_satiation_timeout_msec;
	long stream_congestion_bandwidth_average_gain_bits;
	long stream_congestion_offender_bandwidth_increment_bps;
	long stream_congestion_offender_timeout_msec;
	long stream_growth_maximum_count;
	real stream_growth_maximum_fraction;
	real stream_growth_desire_maximum;
	long stream_growth_desire_delay_multiplier;
	real stream_growth_desire_penalty_bandwidth_multiplier;
	long stream_growth_out_of_game_maximum_rtt_increase_msec;
	long stream_growth_simulation_maximum_rtt_increase_msec;
	long stream_non_growth_simulation_maximum_rtt_increase_msec;
	long stream_growth_backoff_rtt_event_threshold;
	long stream_growth_incremental_rtt_event_threshold;
	long stream_growth_incremental_rtt_increase_msec;
	long stream_cautious_expansion_limited_periods_count;
	long stream_cautious_expansion_instability_timeout_msec;
	long bandwidth_monitor_period_count;
	long bandwidth_monitor_limitation_bps;
	real bandwidth_monitor_constriction_fraction;
	long bandwidth_monitor_constriction_threshold_bps;
	long client_badness_rating_threshold;
	long bad_bandwidth_throughput_threshold;
	bool disable_bad_client_anticheating;
	bool disable_bad_connectivity_anticheating;
	bool disable_bad_bandwidth_anticheating;
	long initial_timeout;
	long mini_period_minimum_duration;
	long mini_period_minimum_rtt_deviation;
	long stream_wants_more_bandwidth_fudge_factor;
	long stream_wants_more_bandwidth_fudge_factor_small;
	long stream_wants_more_allocation_fudge_factor;
	long stream_wants_more_allocation_fudge_factor_small;
	long stream_maximum_instability_value;
	long stream_probe_failure_limit;
	long stream_rebalance_interval_msec;
	long packet_loss_rate_gain_bits;
	long packet_loss_rate_deviation_gain_bits;
	long maximum_rtt_increase_msec;
	long maximum_lost_packet_rate_increase;
	long minimum_packet_loss_deviation;
	long minimum_probe_packet_loss_deviation;
	long stream_maximum_bandwidth_maximum_delta;
	long stream_maximum_bandwidth_skip_max;
	long packet_loss_deviation_adjustment;
	long packet_rate_deviation_adjustment;
	long consecutive_rate_failures_before_badness;
	long consecutive_latency_failures_before_badness;
	long consecutive_packet_loss_failures_before_badness;
	long consecutive_packet_loss_spike_failures_before_badness;
	long badness_minimum_host_to_client_bandwidth;
	long badness_minimum_allocated_host_to_client_bandwidth;
	long badness_minimum_client_to_host_bandwidth;
	long badness_minimum_allocated_client_to_host_bandwidth;
	long badness_maximum_latency_msec;
	long badness_maximum_packet_loss_rate;
	long badness_maximum_packet_loss_spike_count;
	long stream_congestion_rtt_multiplier;
	long remote_client_downstream_usage_multiplier;
	long remote_host_downstream_usage_multiplier;
	long throttle_congested_stream_bandwidth_multiplier;
	long throttle_noncongested_stream_bandwidth_multiplier;
	long bandwidth_estimate_multiplier_reliable;
	long bandwidth_estimate_multiplier_unreliable;
	long bandwidth_estimate_multiplier_qos;
	long qos_to_live_interval_msec;
	long synchronous_joining_maximum_stream_growth_shift;
	long maximum_aggressive_total_growth_bandwidth_bps;
	long maximum_cautious_total_growth_bandwidth_bps;
	long client_probe_base_delay_msec;
	long client_probe_additional_client_delay_msec;
	long client_probe_variability;
	long stream_stability_deviation_multiplier;
	long stream_growth_base_upstream_shift;
	long stream_growth_base_upstream_shift_max;
	long stream_fail_all_failed_related_probes;
	long maximum_consecutive_probe_successes;
	long minimum_packet_rate_for_automatic_congestion;
	long maximum_rtt_for_automatic_congestion;
	bool do_collateral_last_resort_throttle;
	bool release_throttle_lock_on_first_congest;
	bool round_robin_probes;
	bool fail_all_member_probes_together;
	bool mark_throttled_stream_with_one_failure;
	bool check_single_stream_overprobe;
	bool use_deviation_only_for_related_rtt_timeout;
	bool fast_probe_failed_streams;
	long packet_loss_spike_threshold;
	long packet_loss_spike_minimum_packet_count;
	long packet_loss_spike_skip_averaging_any_spike;
	long stable_probe_start_stream_growth_base;
	long stable_probe_start_stream_growth_shift_shift;
	long stable_probe_maximum_stream_growth_shift;
	long stable_probe_query_time_msec;
	long stable_probe_query_time_minimum_msec;
	long stable_probe_settle_time_msec;
	real stable_probe_settle_bandwidth_multiplier;
	long stable_probe_throttle_minimum_rollback;
	long stable_probe_recover_minimum_time_msec;
	long stable_probe_overprobe_minimum_bps;
	long stable_probe_overprobe_maximum_bps;
	long stable_probe_overprobe_transmit_ratio;
	long initial_probe_start_stream_growth_base;
	long initial_probe_start_stream_growth_shift_shift;
	long initial_probe_maximum_stream_growth_shift;
	long initial_probe_query_time_msec;
	long initial_probe_query_time_minimum_msec;
	long initial_probe_settle_time_msec;
	real initial_probe_settle_bandwidth_multiplier;
	long initial_probe_throttle_minimum_rollback;
	long initial_probe_recover_minimum_time_msec;
	long initial_probe_overprobe_minimum_bps;
	long initial_probe_overprobe_maximum_bps;
	long initial_probe_overprobe_transmit_ratio;
	long fast_probe_start_stream_growth_base;
	long fast_probe_start_stream_growth_shift_shift;
	long fast_probe_maximum_stream_growth_shift;
	long fast_probe_query_time_msec;
	long fast_probe_query_time_minimum_msec;
	long fast_probe_settle_time_msec;
	real fast_probe_settle_bandwidth_multiplier;
	long fast_probe_throttle_minimum_rollback;
	long fast_probe_recover_minimum_time_msec;
	long fast_probe_overprobe_minimum_bps;
	long fast_probe_overprobe_maximum_bps;
	long fast_probe_overprobe_transmit_ratio;
	long slow_probe_start_stream_growth_base;
	long slow_probe_start_stream_growth_shift_shift;
	long slow_probe_maximum_stream_growth_shift;
	long slow_probe_query_time_msec;
	long slow_probe_query_time_minimum_msec;
	long slow_probe_settle_time_msec;
	real slow_probe_settle_bandwidth_multiplier;
	long slow_probe_throttle_minimum_rollback;
	long slow_probe_recover_minimum_time_msec;
	long slow_probe_overprobe_minimum_bps;
	long slow_probe_overprobe_maximum_bps;
	long slow_probe_overprobe_transmit_ratio;
};
static_assert(sizeof(s_observer_configuration) == 0x418);

struct s_channel_configuration
{
	long connect_request_interval_msec;
	long connect_request_count;
	long connect_request_timeout_msec;
	long establish_timeout_msec;
	long packet_statistics_interval;
};
static_assert(sizeof(s_channel_configuration) == 0x14);

struct s_connection_configuration
{
	long sequence_advance_rate;
	long discard_ancient_reply_sequence_threshold;
	long packet_skipped_outoforder_threshold;
	long retain_lost_packets_msec;
	long latency_average_gain_bits;
	long latency_deviation_gain_bits;
	long timeout_deviations;
	long timeout_minimum_tolerance_msec;
	long timeout_minimum_msec;
	long inactive_timeout_deviations;
	long inactive_timeout_minimum_tolerance_msec;
	long inactive_timeout_minimum_msec;
	long initial_latency_minimum_msec;
	long initial_latency_average_msec;
	long initial_latency_deviation_msec;
	long initial_timeout_msec;
	long backoff_increment_msec;
	long backoff_max_msec;
	long backoff_decrement_msec;
	real bandwidth_warning_latency_multiplier;
	long bandwidth_warning_latency_minimum_increase;
};
static_assert(sizeof(s_connection_configuration) == 0x54);

struct s_delivery_configuration
{
	s_channel_configuration channel_configuration;
	s_connection_configuration connection;
};
static_assert(sizeof(s_delivery_configuration) == 0x68);

struct s_transport_configuration
{
	long address_resolution_timeout_msec;
	long qos_probe_count[2];
	long qos_probe_bps[2];
	bool qos_upstream_cap_enabled;
	long qos_upstream_cap_upstream_modifier_percentage;
	long qos_upstream_cap_correction_modifier;
};
static_assert(sizeof(s_transport_configuration) == 0x20);

struct s_voice_configuration
{
	real push_to_talk_inactivity_threshold_seconds;
	real maximum_push_to_talk_time_seconds;
	bool clients_can_be_preferred_consumers_of_voice_repeater;
	long open_channel_player_count;
};
static_assert(sizeof(s_voice_configuration) == 0x10);

struct s_data_mine_configuration
{
	long ui_upload_record_threshold;
	long ui_upload_time_threshold;
	bool record_uploads_prevent_game_from_starting;
};
static_assert(sizeof(s_data_mine_configuration) == 0xC);

struct s_griefer_configuration
{
	ulong betrayal_decrement_time;
	ulong eject_decrement_time;
	real betrayal_cutoff;
	long ejection_cutoff;
	real friendly_assist_amount; // UNUSED
};
static_assert(sizeof(s_griefer_configuration) == 0x14);

struct s_network_memory_configuration
{
	long network_heap_large_size;
	long network_heap_small_size;
};
static_assert(sizeof(s_network_memory_configuration) == 0x8);

struct s_user_interface
{
	long allow_matchmaking_abort_interval_msec;
	long networked_playback_maximum_player_count;
	long basic_training_completion_minimum_games_completed;
	long basic_training_completion_minimum_experience;
};
static_assert(sizeof(s_user_interface) == 0x10);

struct s_life_cycle_handler_joining_configuration
{
	long network_session_migration_wait_timeout_msec;
	long network_session_migration_disband_timeout_msec;
	long join_remote_squad_player_reservation_timeout_msec;
	long request_migration_start_rety_interval_msec;
	long request_migration_abort_rety_interval_msec;
	long joining_search_qos_bps;
	long join_timeout_msec;
	long desperation_wait_time_seconds;
};
static_assert(sizeof(s_life_cycle_handler_joining_configuration) == 0x20);
static_assert(OFFSETOF(s_life_cycle_handler_joining_configuration, join_remote_squad_player_reservation_timeout_msec) == 0x08);
static_assert(OFFSETOF(s_life_cycle_handler_joining_configuration, join_timeout_msec) == 0x18);

struct s_life_cycle_handler_matchmaking_configuration
{
	bool perform_nat_check;
	real matchmaking_strict_nat_host_percentage;
	long matchmaking_start_failure_wait_time_ms;
	long matchmaking_find_match_joinable_session_threshold;
	long matchmaking_find_match_join_wait_time_ms;
	long matchmaking_find_match_search_results_stale_ms;
	long matchmaking_gather_join_wait_time_ms;
	long matchmaking_search_give_up_time_seconds;
	long join_request_wait_time_ms;
	bool prepare_map_display_map_during_loading;
	long prepare_map_veto_timer_seconds;
	long prepare_map_minimum_load_time_seconds;
	long prepare_map_countdown_timer_seconds;
	long prepare_map_vetoed_countdown_timer_seconds;
	long prepare_map_veto_failed_countdown_timer_seconds;
	long end_match_write_stats_boot_threshold_seconds;
	long arbitration_wait_for_establishment_and_connectivity_threshold_seconds;
	long arbitration_wait_for_completion_threshold_seconds;
	bool post_match_return_to_pre_game_lobby;
	long post_match_stats_refresh_time;
	long warning_toast_minimum_time_seconds;
};
static_assert(sizeof(s_life_cycle_handler_matchmaking_configuration) == 0x54);

struct s_life_cycle_handler_in_game_configuration
{
	long simulation_aborted_host_delay_ms;
	long simulation_aborted_peer_delay_ms;
};
static_assert(sizeof(s_life_cycle_handler_in_game_configuration) == 0x8);

struct s_life_cycle_configuration
{
	s_life_cycle_handler_joining_configuration joining;
	s_life_cycle_handler_matchmaking_configuration matchmaking;
	s_life_cycle_handler_in_game_configuration in_game;
};
static_assert(sizeof(s_life_cycle_configuration) == 0x7C);

struct s_logic_session_tracker_configuration
{
	long unsuitable_session_cache_count;
	long qos_bps;
	long default_qos_refresh_interval_msec;
	long full_qos_timeout_msec;
	long maximum_target_sessions_per_default_qos_task;
	long maximum_qos_tasks;
};
static_assert(sizeof(s_logic_session_tracker_configuration) == 0x18);

struct s_logic_matchmaking_desirability
{
	long add_to_match_desirability_bonus;
	long combined_player_count_per_player_desirability_bonus;
	long fill_out_match_desirability_bonus;
	long needed_party_size_desirability_bonus;
	long gather_time_bonus_threshold_seconds;
	long gather_time_desirability_bonus;
	long search_time_bonus_threshold_seconds;
	long search_time_desirability_bonus;
	long average_skill_range_for_desirability_bonus;
	long average_skill_desirability_bonus_factor;
	real average_mu_range_for_desirability_bonus;
	long average_mu_desirability_bonus_factor;
	long average_experience_rank_range_for_desirability_bonus;
	long average_experience_rank_desirability_bonus_factor;
	long good_host_desirability_bonus;
	long language_match_desirability_bonus;
	long gamer_region_match_desirability_bonus;
	long gamer_zone_match_desirability_bonus;
	long quitter_match_desirability_bonus;
	long dlc_match_desirability_bonus;
	long maximum_ping_for_desirability_bonus;
	long ping_desirability_bonus_interval;
	long desirability_bonus_per_ping_interval;
};
static_assert(sizeof(s_logic_matchmaking_desirability) == 0x5C);

struct s_logic_matchmaking_seeker_configuration
{
	long session_search_wait_time_ms;
	long failed_session_search_wait_time_ms;
	long final_session_search_query_minimum_time_ms;
	long search_stage_strict_skill_round_limit;
	real search_stage_strict_skill_initial_mu_range;
	real search_stage_strict_skill_final_mu_range;
	long search_stage_strict_skill_initial_desired_skill_range;
	long search_stage_strict_skill_final_desired_skill_range;
	long search_stage_strict_skill_initial_average_skill_range;
	long search_stage_strict_skill_final_average_skill_range;
	long search_stage_strict_skill_initial_average_experience_rank_range;
	long search_stage_strict_skill_connection_threshold_ms;
	long search_stage_skill_round_limit;
	long search_stage_skill_connection_threshold_ms;
	long search_stage_any_round_limit;
	long search_stage_any_final_connection_threshold_ms;
	real search_stage_any_final_mu_range;
	long search_stage_any_final_average_skill_range;
	long search_stage_any_final_desired_skill_range;
};
static_assert(sizeof(s_logic_matchmaking_seeker_configuration) == 0x4C);

struct s_logic_leaderboard_configuration
{
	long consecutive_failed_download_threshold;
	long milliseconds_between_failed_downloads;
	long milliseconds_between_consecutive_failed_downloads;
	long refresh_request_wait_time_ms;
};
static_assert(sizeof(s_logic_leaderboard_configuration) == 0x10);

struct s_session_interface_configuration
{
	ulong peer_properties_interval_msec;
	ulong user_addition_interval_msec;
	ulong user_removal_interval_msec;
	ulong user_properties_interval_msec;
	ulong local_specific_parameter_propagation_msec;
	ulong ready_for_next_match_wait_time_milliseconds;
	ulong stat_replication_wait_threshold_milliseconds;
};
static_assert(sizeof(s_session_interface_configuration) == 0x1C);
static_assert(OFFSETOF(s_session_interface_configuration, peer_properties_interval_msec) == 0x00);

struct s_qos_reply_block_configuration
{
	long qos_listener_update_interval;
	long disabled_qos_reply_bandwidth_bps;
};
static_assert(sizeof(s_qos_reply_block_configuration) == 0x8);

struct s_session_qos_reply_block_configuration
{
	long out_of_game_qos_reply_bandwidth_bps;
	long in_game_qos_reply_bandwidth_bps;
};
static_assert(sizeof(s_session_qos_reply_block_configuration) == 0x8);

struct s_logic_configuration
{
	s_logic_session_tracker_configuration session_tracker;
	s_logic_matchmaking_desirability desirability;
	s_logic_matchmaking_seeker_configuration seeker;
	s_logic_leaderboard_configuration leaderboard;
	s_session_interface_configuration session_interface;
	s_qos_reply_block_configuration base_qos_reply_block;
	s_session_qos_reply_block_configuration squad_qos_reply_block;
	s_session_qos_reply_block_configuration group_qos_reply_block;
	long maximum_players_in_coop;
	long maximum_players_in_forge;
};
static_assert(sizeof(s_logic_configuration) == 0x10C);
static_assert(OFFSETOF(s_logic_configuration, session_interface) == 0xD0);

struct s_banhammer_configuration
{
	long machine_file_refresh_seconds;
	long machine_file_refresh_threshold_seconds;
	long user_file_refresh_seconds;
	long user_file_refresh_threshold_seconds;
	long host_chance_reduction_percentage;
	long idle_controller_timeout_seconds;
};
static_assert(sizeof(s_banhammer_configuration) == 0x18);

struct s_simulation_zoom_relevance
{
	real zoom_0_tolerance;
	real zoom_1_tolerance;
	real zoom_0_relevance_bonus;
	real zoom_1_relevance_bonus;
};
static_assert(sizeof(s_simulation_zoom_relevance) == 0x10);

struct s_simulation_control_relevance
{
	real zero_relevance_distance;
	real max_relevance;
	real min_relevance;
	long min_period;
	long max_period;
};
static_assert(sizeof(s_simulation_control_relevance) == 0x14);

struct s_simulation_position_relevance
{
	real distance_to_player_threshold;
	real aiming_vector_high_tolerance;
	real aiming_vector_medium_tolerance;
	real distance_to_player_medium_tolerance;
};
static_assert(sizeof(s_simulation_position_relevance) == 0x10);

struct s_simulation_netdebug_configuration
{
	long bar_maximum_count;
	long axis_bounds[4][2];
};
static_assert(sizeof(s_simulation_netdebug_configuration) == 0x24);

struct s_simulation_view_configuration
{
	long game_results_update_interval_msec;
	long synchronous_client_block_duration_msec;
};
static_assert(sizeof(s_simulation_view_configuration) == 0x8);

struct s_simulation_shared_configuration
{
	real action_persist_time;
	real simulation_event_projectile_supercombine_request_fraction;
};
static_assert(sizeof(s_simulation_shared_configuration) == 0x8);

struct s_simulation_world_configuration
{
	long maximum_catchup_views;
	long join_timeout;
	long host_join_minimum_wait_time;
	long host_join_timeout;
	long join_total_wait_timeout;
	real pause_game_required_machines_fraction;
	real join_activation_blocking_machines_fraction;
	long maximum_catchup_attempts;
	long catchup_failure_timeout;
	long client_join_failure_count;
	long client_activation_failure_timeout;
	real game_simulation_queue_danger_allocation_size_percentage;
	real game_simulation_queue_danger_allocation_count_percentage;
};
static_assert(sizeof(s_simulation_world_configuration) == 0x34);

struct s_simulation_event_configuration
{
	real constant_priority;
	dword cancel_timer_milliseconds;
	real zero_relevance_distance;
	real minimum_priority;
	real maximum_priority;
};
static_assert(sizeof(s_simulation_event_configuration) == 0x14);

struct s_simulation_entity_creation_configuration
{
	real constant_priority;
	real creation_zero_relevance_distance;
	real creation_minimum_priority;
	real creation_maximum_priority;
};
static_assert(sizeof(s_simulation_entity_creation_configuration) == 0x10);

struct s_simulation_entity_update_configuration
{
	real constant_priority;
	real zero_relevance_distance;
	real minimum_relevance;
	real maximum_relevance;
	long minimum_period;
	long maximum_period;
	real normal_minimum_priority;
	real normal_maximum_priority;
	long delayed_time_threshold;
	real delayed_minimum_priority;
	real delayed_maximum_priority;
	real maximum_priority;
	real player_priority;
	real dead_priority;
	real in_motion_by_unit;
};
static_assert(sizeof(s_simulation_entity_update_configuration) == 0x3C);

struct s_simulation_entity_configuration
{
	s_simulation_entity_creation_configuration creation_configuration;
	s_simulation_entity_update_configuration update_configuration;
};
static_assert(sizeof(s_simulation_entity_configuration) == 0x4C);

struct s_simulation_warping_configuration
{
	real simulation_position_update_object_corrective_accept_tolerance;
	real simulation_position_update_object_predicted_accept_tolerance;
	real simulation_position_update_vehicle_corrective_accept_tolerance;
	real simulation_position_update_vehicle_predicted_accept_tolerance;
	real position_update_recent_seconds;
	real position_update_minimum_distance;
};
static_assert(sizeof(s_simulation_warping_configuration) == 0x18);

struct s_simulation_weapon_configuration
{
	real trigger_recent_spew_time;
	real prediction_delay_timer;
	real predicted_fire_allow_ratio;
	real predicted_fire_always_allow_threshold;
};
static_assert(sizeof(s_simulation_weapon_configuration) == 0x10);

struct s_simulation_configuration
{
	s_simulation_zoom_relevance zoom_relevance;
	s_simulation_control_relevance control_relevance;
	s_simulation_position_relevance position_relevance;
	s_simulation_netdebug_configuration netdebug;
	s_simulation_view_configuration view;
	s_simulation_shared_configuration shared;

	// odst?
	real __unknown68;

	s_simulation_world_configuration world;
	c_static_array<s_simulation_event_configuration, k_simulation_event_type_count> simulation_event_configurations;
	c_static_array<s_simulation_entity_configuration, k_simulation_entity_type_count> simulation_entity_configurations;
	s_simulation_warping_configuration warping;
	s_simulation_weapon_configuration weapon;
};
static_assert(sizeof(s_simulation_configuration) == 0xA98);
static_assert(0xA0 == OFFSETOF(s_simulation_configuration, simulation_event_configurations));

struct s_event_manager_view_configuration
{
	long replication_event_maximum_blocked_time;
};
static_assert(sizeof(s_event_manager_view_configuration) == 0x4);

struct s_replication_control_view
{
	real base_non_player_motion_priority;
	long max_priority_threshold;
	real max_priority;
	real medium_priority_base;
	real medium_priority_relevance_scale;
	real min_priority_base;
	real min_priority_relevance_scale;
};
static_assert(sizeof(s_replication_control_view) == 0x1C);

struct s_replication_configuration
{
	s_event_manager_view_configuration event_manager_view;
	s_replication_control_view replication_control_view;
	long maximum_requests_to_send_in_one_frame;
};
static_assert(sizeof(s_replication_configuration) == 0x24);

struct s_map_information
{
	enum e_map_id map_id;
	long map_status;
};
static_assert(sizeof(s_map_information) == 0x8);

struct s_skill_level_configuration
{
	long bonus_skill_level_wins_required[50];
};
static_assert(sizeof(s_skill_level_configuration) == 0xC8);

struct s_experience_configuration
{
	long experience_for_rank[56];
	long skill_for_rank[14];
};
static_assert(sizeof(s_experience_configuration) == 0x118);

struct s_alpha_configuration
{
	bool disable_game;
	bool disable_custom_games;
	long ui_level;
	long maximum_multiplayer_split_screen;
};
static_assert(sizeof(s_alpha_configuration) == 0xC);

struct s_crash_handling_configuration
{
	bool display_crash_handling_ui;
	long minidump_generation;
};
static_assert(sizeof(s_crash_handling_configuration) == 0x8);

struct s_lsp_configuration
{
	long port_range_start;
	long port_range_count;
	long server_retry_count;
	long search_results_fresh_milliseconds;
	long recent_activity_milliseconds;
};
static_assert(sizeof(s_lsp_configuration) == 0x14);

struct s_map_configuration
{
	c_static_array<s_map_information, 32> map_list;
};

struct s_chicken_switches
{
	bool allow_no_hdd_network_coop;
	bool allow_matched_hdd_network_coop;
	bool disallow_cross_language_coop;
	bool disable_prefer_good_connection_changes;
};
static_assert(sizeof(s_chicken_switches) == 0x4);

struct s_determinism_configuration
{
	long determinism_version;
	long determinism_compatible_version;
};
static_assert(sizeof(s_determinism_configuration) == 0x8);

struct s_network_configuration
{
	s_network_file_download_configuration config_download;
	s_bandwidth_configuration bandwidth;
	s_life_cycle_configuration life_cycle;
	s_logic_configuration logic;
	s_banhammer_configuration banhammer;
	s_simulation_configuration simulation_configuration;
	s_replication_configuration replication;
	s_session_configuration session;
	s_observer_configuration observer_configuration;
	s_delivery_configuration delivery;
	s_transport_configuration transport;
	s_voice_configuration voice;
	s_data_mine_configuration data_mine;
	s_griefer_configuration griefer_config;
	s_network_memory_configuration memory;
	s_user_interface user_interface;
	s_skill_level_configuration skill_level_configuration;
	s_experience_configuration experience_configuration;
	s_alpha_configuration alpha_configuration;
	s_crash_handling_configuration crash_handling_configuration;
	s_lsp_configuration lsp_configuration;
	s_map_configuration map_configuration;
	s_chicken_switches chicken_switches;
	s_determinism_configuration determinism_configuration;
};
static_assert(sizeof(s_network_configuration) == 0x1700);
static_assert(0x244 == OFFSETOF(s_network_configuration, life_cycle));
static_assert(0x2C0 == OFFSETOF(s_network_configuration, logic));
static_assert(0x3E4 == OFFSETOF(s_network_configuration, simulation_configuration));
static_assert(0x16F8 == OFFSETOF(s_network_configuration, determinism_configuration));

extern s_network_configuration& g_network_configuration;
extern bool& g_network_configuration_initialized;

s_network_configuration* get_network_configuration();