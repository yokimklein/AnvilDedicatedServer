#pragma once
#include <cseries\cseries.h>
#include <networking\delivery\network_channel.h>
#include <networking\logic\network_bandwidth.h>
#include <networking\transport\transport_security.h>
#include <networking\network_statistics.h>

constexpr long k_network_maximum_observers = 34; // 32 in h3debug, 33 in mcc

enum e_network_observer_owner
{
	_network_observer_owner_squad_one = 0,
	_network_observer_owner_squad_two,
	_network_observer_owner_group,
	_network_observer_owner_simulation,

	k_network_observer_owner_count
};

class c_network_session;
class c_network_link;
class c_network_message_gateway;
class c_network_message_handler;
class c_network_message_type_collection;
struct s_observer_configuration;
struct s_network_message_connect_request;

struct s_channel_observer_owner
{
	c_network_session* owner;
	long managed_session_index;
};
static_assert(sizeof(s_channel_observer_owner) == 0x8);
static_assert(0x0 == OFFSETOF(s_channel_observer_owner, owner));
static_assert(0x4 == OFFSETOF(s_channel_observer_owner, managed_session_index));

class c_network_channel_owner
{
public:
	virtual bool desire_channel_heartbeat(long observer_channel_index) const;
	virtual bool channel_is_load_bearing(long observer_channel_index) const;
	virtual bool attempt_channel_reconnection(long observer_channel_index, bool preexisting_connection) const;
	virtual void notify_channel_connection(long observer_channel_index, dword connection_identifier, bool connected);
	virtual void notify_channel_died(long observer_channel_index);
};
static_assert(sizeof(c_network_channel_owner) == 0x4);

class c_network_observer
{
	public:
		struct s_stream_parameters
		{
			bool active;
			bool is_load_bearing;
			bool is_simulation;
			bool is_simulation_authority;
			bool is_simulation_client;
			bool is_synchronous;
			bool is_synchronous_joining;
			bool saturate_stream;
			long current_bandwidth_bps;
			real current_packet_rate;
			bool current_packet_rate_limiting;
			bool period_bandwidth_restricted;
			long period_transmission_bytes;
			long period_throughput_bytes;
			long period_minimum_average_rtt_msec;
			long period_maximum_average_rtt_msec;
			long period_minimum_lost_packet_rate;
			long period_maximum_lost_packet_rate;
			long period_start_average_rtt_msec;
			long mini_period_initial_rtt_msec;
			long mini_period_initial_lost_packet_rate;
			long mini_period_delivered_packet_count;
			long mini_period_lost_packet_count;
			long mini_period_maximum_average_rtt;
			long mini_period_maximum_average_rtt_fast_deviation;
			long mini_period_last_maximum_average_rtt;
			long mini_period_bits_transmitted_game;
			long mini_period_bits_transmitted_fill;
			long mini_period_bits_transmitted_voice;
			long packet_data_game_ratio;
			long packet_data_fill_ratio;
			long packet_data_voice_ratio;
			long average_lost_packet_rate_unshifted;
			long average_lost_packet_rate;
			long average_lost_packet_rate_deviation;
			bool recent_period_bandwidth_restricted;
			long recent_period_transmission_bps;
			long recent_period_throughput_bps;
			long probe_maximum_rtt_msec;
			long related_probe_maximum_rtt_msec;
			long congest_at_rtt_msec;
			long congest_at_lost_packet_rate;
			bool congest_at_rtt_locked;
			long last_known_good_lost_packet_rate;
			long last_known_good_lost_packet_rate_deviation;
			long last_known_good_lost_packet_rate_bandwidth_bps;
			bool has_completed_initial_probe;
			bool has_set_initial_bandwidth;
			long overprobe_bps;
			long allocated_bandwidth_bps;
			long maximum_bandwidth_bps;
			long maximum_bandwidth_find_duplicate_count;
			dword time_to_grow_bandwidth_base;
			long time_to_grow_bandwidth_delta;
			long packet_loss_spike_count;
			long upstream_bandwidth_failures;
			long downstream_bandwidth_failures;
			long latency_failures;
			long packet_loss_failures;
			long packet_loss_spike_failures;
			long consecutive_probe_failures;
			long consecutive_probe_successes;
			long consecutive_congested_bandwidth_throttles;
			long probe_previous_bandwidth;
			bool stream_requires_more_bandwidth;
		};
		static_assert(sizeof(s_stream_parameters) == 0xE4);

		struct s_channel_observer
		{
			c_network_channel channel;
			c_enum<e_observer_state, long, _observer_state_none, k_observer_state_count> state;
			ulong state_entry_timestamp;
			byte_flags flags;
			byte_flags owner_flags;
			short connection_attempts;
			ulong owner_connection_identifier;
			s_transport_secure_address remote_secure_address;
			ulong secure_connection_discarded_owner_mask;
			long secure_connection_owner;
			transport_address secure_connection_address;
			dword last_acknowledgement_timestamp; // -1 when allocated
			long average_rtt_msec; // -1 when allocated
			long average_rtt_fast_deviation_msec; // set by a global value that is 1000 by default, likely to be a timeout?
			long average_rtt_deviation_msec; // set by a global value that is 1000 by default, likely to be a timeout?
			long average_rtt_interval_msec; // 0
			long current_timeout_msec;
			//bool is_bad_client;
			//long remote_machine_upstream_bandwidth_bps;
			//long remote_machine_downstream_bandwidth_bps;
			//long qos_attempt_index;
			//s_transport_qos_result qos_result;
			ulong last_inactive_timestamp;
			ulong undesired_connection_timestamp;
			ulong undesired_connection_empty_timestamp;
			c_network_time_statistics received_bytes_statistics;
			c_network_time_statistics transmitted_bytes_statistics;
			c_network_time_statistics acknowledged_bytes_statistics;
			c_network_window_statistics packet_loss_statistics;
			c_network_window_statistics packet_warning_statistics;
			long bandwidth_monitor_interval_count;
			long bandwidth_monitor_period_count;
			long bandwidth_monitor_throughput_minimum_bps;
			long bandwidth_monitor_throughput_maximum_bps;
			long bandwidth_monitor_previous_maximum_bps;
			long bandwidth_event_counters[5];
			dword last_packet_slot_timestamp;
			c_network_observer::s_stream_parameters stream;
			bool badness_reported_connectivity;
			bool badness_reported_squad_host;
			bool badness_reported_squad_client;
			bool badness_reported_group_host;
			bool badness_reported_group_client;
			bool badness_was_ever_game_squad_host;
			bool badness_was_ever_game_squad_client;
			bool badness_was_ever_game_group_host;
			bool badness_was_ever_game_group_client;
			qword message_backlog_mask;
		};
		static_assert(sizeof(s_channel_observer) == 0x10A8); // 0x10D8 in ms23
		static_assert(0xFAC == OFFSETOF(s_channel_observer, stream.active));
		static_assert(0xFAE == OFFSETOF(s_channel_observer, stream.is_simulation));

		void handle_connect_request(transport_address const* address, s_network_message_connect_request const* message);
		void observer_channel_initiate_connection(e_network_observer_owner observer_owner, long observer_channel_index);
		const char* get_name(long observer_index);
		bool observer_channel_dead(e_network_observer_owner owner_type, long observer_index);
		s_channel_observer* get_observer(e_network_observer_owner owner_type, long observer_index);
		void observer_channel_send_message(e_network_observer_owner owner_type, long observer_index, bool out_of_band, e_network_message_type message_type, long message_size, void const* message);
		bool observer_channel_connected(e_network_observer_owner owner_type, long observer_index);
		bool observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void quality_statistics_get_ratings(ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating);
		long observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel const* channel) const; // owner type might be a byte?
		s_channel_observer const* find_observer_by_channel(c_network_channel const* channel) const;
		bool is_bandwidth_stable();
		void quality_statistics_notify_peer_left_gracefully(e_network_observer_owner owner, long observer_index);
		void quality_statistics_report_badness(long observer_index, bool bad_not_good);

		c_network_link* m_link;
		c_network_message_gateway* m_message_gateway;
		c_network_message_handler* m_message_handler;
		c_network_message_type_collection* m_message_types;
		s_observer_configuration* m_configuration;
		s_channel_observer_owner m_owners[k_network_observer_owner_count];
		c_network_session* m_session;
		s_channel_observer m_channel_observers[k_network_maximum_observers];
		bool m_quality_statistics_are_set;
		s_network_quality_statistics m_quality_statistics;
		long m_expansion_state;
		long m_probe_state;
		bool m_prioritize_upload_bandwidth;
		bool m_online_network_environment;
		bool m_bandwidth_estimate_reliable;
		bool m_allow_bandwidth_estimate_override;
		long m_bandwidth_estimated_bps;
		long m_bandwidth_maximum_throughput_bps;
		long m_bandwidth_congestion_bps;
		dword m_bandwidth_satiation_timestamp;
		bool m_bandwidth_satiation_setting;
		long m_bandwidth_satiation_count[2];
		long m_maximum_upstream_bandwidth_bps;
		bool m_has_simulation_stream;
		dword m_recently_probed_streams;
		dword m_time_of_expansion;
		dword m_time_of_probe;
		dword m_time_of_probe_delay;
		dword m_time_of_end_of_last_probe;
		long m_time_to_delay_next_probe;
		long m_bandwidth_known_good_kbps;
		long m_bandwidth_known_bad_kbps;
		long m_bandwidth_target_kbps;
		c_network_time_statistics m_bandwidth_statistics;
		long m_allocated_observer_count;
		long m_allocated_bandwidth_bps;
		real m_allocated_rate_authority;
		real m_allocated_rate_client;
		long m_allocated_upstream_bandwidth;
		long m_allocated_upload_bandwidth;
		dword m_stream_period_start_timestamp;
		dword m_stream_mini_period_start_timestamp;
		bool m_stream_needs_rebalancing;
		bool m_stream_abort_growth;
		bool m_stream_period_restart_flag;
		bool m_stream_period_restart_abort_expansion_flag;
		bool m_stream_period_restart_record_congestion_flag;
		long m_stream_period_growth_initiated_count;
		long m_stream_period_growth_maximum_count;
		dword m_stream_reset_timestamp;
		bool m_stream_all_streams_are_stable;
		dword m_streams_that_are_still_growing;
};
static_assert(sizeof(c_network_observer) == 0x238B8);
static_assert(0x14 == OFFSETOF(c_network_observer, m_owners));
static_assert(0x38 == OFFSETOF(c_network_observer, m_channel_observers));
static_assert(0x23688 == OFFSETOF(c_network_observer, m_quality_statistics_are_set));
static_assert(0x23690 == OFFSETOF(c_network_observer, m_quality_statistics));
static_assert(0x236D0 == OFFSETOF(c_network_observer, m_quality_statistics.connectivity_history));
static_assert(0x23690 == OFFSETOF(c_network_observer, m_quality_statistics.session[0].client_badness_history[0]));
static_assert(0x236E0 == OFFSETOF(c_network_observer, m_quality_statistics.bandwidth_data));
static_assert(0x23748 == OFFSETOF(c_network_observer, m_expansion_state));
