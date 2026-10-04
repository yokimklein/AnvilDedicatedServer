#include "game_allegiance.h"

bool __fastcall game_team_is_enemy(e_game_team team1, e_game_team team2)
{
    return INVOKE(ADDRESS_GAME_TEAM_IS_ENEMY, game_team_is_enemy, team1, team2);
}