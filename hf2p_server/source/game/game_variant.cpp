#include "game_variant.h"

c_game_engine_base_variant* c_game_variant::get_active_variant()
{
	return &this->m_storage.m_base_variant;
}