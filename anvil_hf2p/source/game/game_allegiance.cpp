#include "game_allegiance.h"

bool __fastcall game_team_is_enemy(e_game_team team1, e_game_team team2)
{
    return INVOKE(0x171070, game_team_is_enemy, team1, team2);
}