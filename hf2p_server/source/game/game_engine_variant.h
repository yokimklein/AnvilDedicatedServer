#pragma once

#include <cseries\cseries.h>
#include <game\game_engine_traits.h>
#include <saved_games\saved_game_files.h>
#include <memory\bitstream.h>
#include <game\game_engine_ctf.h>
#include <game\game_engine_slayer.h>
#include <game\game_engine_oddball.h>
#include <game\game_engine_king.h>
#include <game\game_engine_sandbox.h>
#include <game\game_engine_vip.h>
#include <game\game_engine_juggernaut.h>
#include <game\game_engine_territories.h>
#include <game\game_engine_assault.h>
#include <game\game_engine_infection.h>

enum e_game_engine_type
{
	_game_engine_type_none = 0,
	_game_engine_type_ctf,
	_game_engine_type_slayer,
	_game_engine_type_oddball,
	_game_engine_type_king,
	_game_engine_type_sandbox,
	_game_engine_type_vip,
	_game_engine_type_juggernaut,
	_game_engine_type_territories,
	_game_engine_type_assault,
	_game_engine_type_infection,

	k_game_engine_type_count,
	k_game_engine_type_default = _game_engine_type_none
};

class c_game_variant
{
public:
	c_game_variant()
	{
		m_game_engine_index = 0;
		csmemset(m_storage.variant_data, 0, sizeof(this->m_storage));
	};
	c_game_engine_base_variant* get_active_variant();

	c_enum<e_game_engine_type, long, k_game_engine_type_count> m_game_engine_index;
	union game_engine_variants {
		game_engine_variants()
		{
			memset(this, 0, 0x260);
		};

		c_game_engine_base_variant m_base_variant;
		c_game_engine_ctf_variant m_ctf_variant;
		c_game_engine_slayer_variant m_slayer_variant;
		c_game_engine_oddball_variant m_oddball_variant;
		c_game_engine_king_variant m_king_variant;
		c_game_engine_sandbox_variant m_sandbox_variant;
		c_game_engine_vip_variant m_vip_variant;
		c_game_engine_juggernaut_variant m_juggernaut_variant;
		c_game_engine_territories_variant m_territories_variant;
		c_game_engine_assault_variant m_assault_variant;
		c_game_engine_infection_variant m_infection_variant;
		byte variant_data[0x260];
	} m_storage;
};
static_assert(sizeof(c_game_variant) == 0x264);

//extern const char* game_engine_variant_get_name(long game_engine_variant);
c_game_variant* __fastcall build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index);