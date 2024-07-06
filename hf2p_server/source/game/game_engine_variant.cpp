#include "game_engine_variant.h"

c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options()
{
	return &this->m_miscellaneous_options;
}

c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options()
{
	return &this->m_respawn_options;
}

c_game_engine_social_options* c_game_engine_base_variant::get_social_options()
{
	return &this->m_social_options;
}

c_game_variant* __fastcall build_default_game_variant(c_game_variant* game_variant, e_game_engine_type engine_variant)
{
	return INVOKE(0xE9BE0, build_default_game_variant, game_variant, engine_variant);
}