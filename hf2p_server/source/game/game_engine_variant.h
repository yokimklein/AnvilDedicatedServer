#pragma once

#include "..\cseries\cseries.h"
#include "game_engine_traits.h"
#include "..\saved_games\saved_game_files.h"
#include "..\memory\bitstream.h"

enum e_game_engine_type
{
	_game_engine_base_variant = 0,
	_game_engine_ctf_variant,
	_game_engine_slayer_variant,
	_game_engine_oddball_variant,
	_game_engine_king_variant,
	_game_engine_sandbox_variant,
	_game_engine_vip_variant,
	_game_engine_juggernaut_variant,
	_game_engine_territories_variant,
	_game_engine_assault_variant,
	_game_engine_infection_variant,

	k_game_engine_type_count,
	k_game_engine_type_default = _game_engine_base_variant
};

#pragma pack(push, 4)
class c_game_engine_base_variant
{
public:
	c_game_engine_base_variant() :
		__unknown4(),
		m_name(),
		m_metadata(),
		m_miscellaneous_options(),
		m_respawn_options(),
		m_social_options(),
		m_map_override_options(),
		m_flags(),
		m_team_scoring_method()
	{
	};

	virtual long __cdecl get_game_engine_name_string_id();
	virtual long __thiscall get_game_engine_default_description_string_id();
	virtual void __thiscall initialize();
	virtual void __thiscall validate();
	virtual void __thiscall encode(c_bitstream*);
	virtual void __thiscall decode(c_bitstream*);
	virtual bool __cdecl can_add_to_recent_list();
	virtual long __thiscall get_score_to_win_round();
	virtual long __thiscall get_score_unknown(); // halo online specific
	virtual bool __stdcall can_be_cast_to(e_game_engine_type, void const**);
	virtual void __stdcall custom_team_score_stats(long, long, long);

	c_game_engine_miscellaneous_options* get_miscellaneous_options();
	c_game_engine_respawn_options* get_respawn_options();

	dword __unknown4;
	string m_name;
	s_saved_game_item_metadata m_metadata;
	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;
	word_flags m_flags;
	short m_team_scoring_method;
};
static_assert(sizeof(c_game_engine_base_variant) == 0x1D0);
#pragma pack(pop)

//extern const char* game_engine_variant_get_name(long game_engine_variant);