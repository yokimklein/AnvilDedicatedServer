#pragma once

class c_animation_channel_base
{
	char data[0x14];
};
static_assert(sizeof(c_animation_channel_base) == 0x14);

class c_animation_channel : public c_animation_channel_base
{
	char data[0x28];
};
static_assert(sizeof(c_animation_channel) == 0x3C);