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
    s_modifier(e_modifiers modifier, float value);
    s_modifier(std::string modifier, float value);

    e_modifiers modifier;
    float value;
};

struct s_cached_armor_item
{
    s_cached_armor_item(s_title_instance& instance);


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

    // $TODO: WHEN USING THESE TO SERIALIZE LOADOUTS, MAKE SURE YOU CHECK THAT THE INDICES AREN'T -1 BEFORE YOU USE THEM!! (except for weapons, that's okay)
    std::map<std::string, s_cached_armor_item> armor_items;
    std::map<std::string, e_weapon> weapons;
    std::map<std::string, e_grenade> grenades;
    std::map<std::string, std::vector<s_modifier>> boosters;
    std::map<std::string, e_tactical_package> consumables;
    std::map<std::string, ulong> colours;

    // scoring event - set this directly in runtime tags, no need to cache
};
extern s_backend_data_cache g_backend_data_cache;