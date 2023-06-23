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