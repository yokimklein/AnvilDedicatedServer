#pragma once
#include <cseries\cseries.h>
#include <anvil\backend\user.h>
#include <anvil\backend\lobby.h>
#include <game\player_appearance.h>
#include <hf2p\loadouts.h>
#include <vector>
#include <string>
#include <map>
#include <mutex>

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

struct s_cached_scoring_event
{
    ulong event_index;
    short xp_reward;
};

struct s_cached_public_data
{
    s_cached_public_data();

    s_backend_loadout loadouts[k_maximum_loadouts];
    s_backend_customisation customisation;
};

class c_backend_data_cache
{
public:
    void clear_title_instances();

    void user_data_remove(qword user_id);
    s_backend_loadout* const loadout_get(qword user_id, long loadout_index);
    void loadout_cache(qword user_id, s_backend_loadout* const loadout, long loadout_index);
    s_backend_customisation* const customisation_get(qword user_id);
    void customisation_cache(qword user_id, s_backend_customisation* const customisation);

    bool m_valid = false;
    qword m_last_update = NONE;

    s_lobby_info m_lobby_info;
    s_lobby_session_data m_lobby_session;

    // Title instances
    // $TODO: WHEN USING THESE TO SERIALIZE LOADOUTS, MAKE SURE YOU CHECK THAT THE INDICES AREN'T -1 BEFORE YOU USE THEM!! (except for weapons, that's okay)
    std::map<std::string, s_cached_armor_item> m_armor_items;
    std::map<std::string, e_weapon> m_weapons;
    std::map<std::string, e_grenade> m_grenades;
    std::map<std::string, std::vector<s_modifier>> m_boosters;
    std::map<std::string, e_tactical_package> m_consumables;
    std::map<std::string, ulong> m_colours;
    std::vector<s_cached_scoring_event> m_scoring_events;

private:
    // cached public data - private behind a mutex as this is written to by the backend thread, and cleared by the game thread
    std::map<qword, s_cached_public_data> m_public_data;
    std::mutex m_public_data_mutex;
};
extern c_backend_data_cache g_backend_data_cache;