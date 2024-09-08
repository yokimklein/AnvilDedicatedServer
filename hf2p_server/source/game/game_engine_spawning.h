#pragma once
#include <memory\data.h>

constexpr long k_pre_round_seconds = 18; // 18 in ms29

long game_engine_get_pre_round_ticks();
bool __fastcall game_engine_should_spawn_player(datum_index player_index);