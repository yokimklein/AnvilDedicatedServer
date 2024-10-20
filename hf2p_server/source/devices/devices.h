#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <objects\objects.h>
#include <animations\mixing_board\channels\animation_channel.h>

struct s_device_animation_control
{
    float __unknown0;
    float __unknown4;
    float __unknown8;
    float __unknownC;
    float __unknown10;
    float __unknown14;
    float __unknown18;
    float __unknown1C;
};

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
    short __unknown19A;
    s_device_animation_control position_track;
    s_device_animation_control overlay_track;
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
	real __unknown4; // desired value, actual value
	real __unknown8; // desired value, actual value
	dword __unknownC;
};
static_assert(sizeof(device_group_datum) == 0x10);