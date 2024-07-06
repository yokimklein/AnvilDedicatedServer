#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

struct c_candy_spawner
{
	byte_flags m_flags;
	byte __pad1[0x3];
	long m_map_variant_index;
	long m_last_object_spawned_index;
	word m_respawn_timer_seconds;
	byte __padE[0x2];
};
static_assert(sizeof(c_candy_spawner) == 0x10);

struct c_candy_monitor
{
	long m_spawned_object_index;
	word m_abandoned_seconds;
	bool m_disturbed_from_initial_placement;
	bool __unknown7;
	byte __pad8[0x2];
	word __unknown_time; // creation time?
};
static_assert(sizeof(c_candy_monitor) == 0xC);

struct c_multiplayer_candy_monitor_manager
{
	c_candy_spawner m_spawners[512];
	c_candy_monitor m_monitors[512];
};
static_assert(sizeof(c_multiplayer_candy_monitor_manager) == 0x3800);

void __fastcall game_engine_register_object(datum_index object_index);