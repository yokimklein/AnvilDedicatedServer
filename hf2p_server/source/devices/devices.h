#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <objects\objects.h>
#include <animations\mixing_board\channels\animation_channel.h>

struct s_device_animation_control
{
	real current_position;
	real current_velocity;
	real time_remaining;
	real cruise_start_time;
	real cruise_end_time;
	real cruise_velocity;
	real exit_velocity;
	real exit_pos;
};
static_assert(sizeof(s_device_animation_control) == 0x20);

struct _device_datum
{
    dword_flags flags;
    long power_group_index;
    float power;
    float power_velocity;
    long position_group_index;
    float position;
    float position_velocity;
    short delay_ticks;
    word pad;
    s_device_animation_control position_track;
    s_device_animation_control power_and_overlay_track;
    c_animation_channel position_channel;
    c_animation_channel power_and_overlay_track_channel;
};
static_assert(sizeof(_device_datum) == 0xD8);

struct device_datum
{
    long definition_index;
    _object_datum object;
    _device_datum device;
};
static_assert(sizeof(device_datum) == 0x254);

struct device_group_datum : s_datum_header
{
	word_flags flags;
    real desired_value;
    real initial_value;
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);