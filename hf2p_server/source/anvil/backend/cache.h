#pragma once
#include <anvil\backend\user.h>
#include <anvil\backend\lobby.h>

struct s_backend_data_cache
{
    s_lobby_info lobby_info;
    s_lobby_session_data lobby_session;

    // TIs
    // 
    // armor_item
    // weapon
    // grenade
    // booster
    // consumable
    // color
    // scoring event
};
extern s_backend_data_cache g_backend_data_cache;