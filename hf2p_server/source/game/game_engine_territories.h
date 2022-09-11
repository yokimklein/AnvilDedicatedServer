#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_default.h"
#include "game_engine_territories_traits.h"
#include "game_engine_variant.h"

class c_game_engine_territories_variant : c_game_engine_base_variant
{
public:
	c_game_engine_territories_variant() :
		m_variant_flags(),
		m_respawn_on_capture(),
		m_capture_time(),
		m_sudden_death_time(),
		m_defender_traits(),
		m_attacker_traits(),
		unused()
	{
	};

	c_flags<e_territories_variant_flags, word_flags, k_territories_variant_flags> m_variant_flags;

	c_enum<e_territories_respawn_on_capture_settings, short, k_territories_respawn_on_capture_settings> m_respawn_on_capture;
	c_enum<e_territories_capture_time_settings, short, k_territories_capture_time_settings> m_capture_time;
	c_enum<e_territories_sudden_death_settings, short, k_territories_sudden_death_settings> m_sudden_death_time;

	c_player_traits m_defender_traits;
	c_player_traits m_attacker_traits;

	byte unused[0x50];
};
static_assert(sizeof(c_game_engine_territories_variant) == 0x260);

struct c_territories_engine : c_game_engine
{
};