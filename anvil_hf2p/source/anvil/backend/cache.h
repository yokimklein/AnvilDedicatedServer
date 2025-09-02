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
    s_cached_armor_item();
    s_cached_armor_item(s_title_instance& instance);

    std::vector<e_armor> gender_armor;
    long race_id;
    std::vector<s_modifier> modifiers;
    bool can_colour_regions[k_armor_colors_count];
};

struct s_cached_consumable_costs
{
    long energy_cost = 0;
    float cooldown = 0.0f;
    float cooldown_init = 0.0f;
};

struct s_cached_consumable
{
    e_consumables consumable_index;
    s_cached_consumable_costs costs;
};

struct s_cached_scoring_event
{
    ulong event_index;
    short xp_reward;
};

struct s_cached_public_data
{
    s_cached_public_data();

    void write_configuration(s_s3d_player_container* out_container, s_s3d_player_customization* out_customisation, s_player_appearance* out_appearance);

    s_backend_loadout loadouts[k_maximum_loadouts];
    s_backend_customisation customisation;
};

class c_backend_data_cache
{
public:
    static void clear_title_instances();
    static void refresh_consumable_costs();
    static void refresh_scoring_events();
    static void reset_earned_wp_events();
    static void cache_wp_event(s_game_engine_event_data* event_data);

    s_cached_public_data* const public_data_get(qword user_id);
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
    std::map<std::string, s_cached_armor_item> m_armor_items;
    std::map<std::string, e_weapon> m_weapons;
    std::map<std::string, e_grenade> m_grenades;
    std::map<std::string, std::vector<s_modifier>> m_boosters;
    std::map<std::string, s_cached_consumable> m_consumables;
    std::map<std::string, ulong> m_colours;
    std::vector<s_cached_scoring_event> m_scoring_events;

    // refreshed on map load, size of multiplayer\multiplayer_globals.multiplayer_globals > Universal[0].Equipment
    std::vector<s_cached_consumable_costs> m_consumable_costs;

    // reset on map load
    std::vector<ulong> m_earned_wp_events[k_network_maximum_players_per_session];

private:
    // cached public data - private behind a mutex as this is written to by the backend thread, and cleared by the game thread
    std::map<qword, s_cached_public_data> m_public_data;
    std::mutex m_public_data_mutex;
};
extern c_backend_data_cache g_backend_data_cache;
