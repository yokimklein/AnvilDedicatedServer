#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

struct c_candy_spawner
{
	char m_flags;
	long m_map_variant_index;
	long m_last_object_spawned_index;
	short m_respawn_timer_seconds;
	short pad;
};
static_assert(sizeof(c_candy_spawner) == 0x10);

struct c_candy_monitor
{
	long m_spawned_object_index;
	short m_abandoned_seconds;
	bool m_disturbed_from_initial_placement;
	bool m_pad[3];
	short m_abandonment_timeout;
};
static_assert(sizeof(c_candy_monitor) == 0xC);

struct c_multiplayer_candy_monitor_manager
{
	enum
	{
		k_dummy_spawn_monitor_index = 0x1971,
		k_maximum_number_of_monitored_pieces_of_candy = 512,
		k_insignificant_object_abandonment_threshold_seconds = 10,
	};

	c_static_array<c_candy_spawner, k_maximum_number_of_monitored_pieces_of_candy> m_candy_spawners;
	c_static_array<c_candy_monitor, k_maximum_number_of_monitored_pieces_of_candy> m_candy_monitors;
};
static_assert(sizeof(c_multiplayer_candy_monitor_manager) == 0x3800);

void __fastcall game_engine_register_object(datum_index object_index);