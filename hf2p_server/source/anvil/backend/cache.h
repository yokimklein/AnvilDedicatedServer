#pragma once
#include <cseries\cseries.h>
#include <anvil\backend\user.h>
#include <anvil\backend\lobby.h>
#include <game\player_appearance.h>
#include <vector>
#include <string>
#include <map>

struct s_title_instance;

struct s_modifier
{
    e_modifiers modifier;
    float value;
};

struct s_armor_item
{
    s_armor_item(s_title_instance& instance);


    std::vector<e_armor> gender_armor;
    long race_id;
    std::vector<s_modifier> modifiers;
    bool can_colour_regions[k_armor_colors_count];
};

struct s_backend_data_cache
{
    void clear_title_instances();

    bool valid = false;
    qword last_update = NONE;

    s_lobby_info lobby_info;
    s_lobby_session_data lobby_session;

    std::map<std::string, s_armor_item> armor_items;
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