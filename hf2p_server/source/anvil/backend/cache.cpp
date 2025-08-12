#include "cache.h"
#include <anvil\backend\services\title_resource_service.h>
#include <scenario\scenario.h>
#include <game\game_globals.h>
#include <cache\cache_files.h>
#include <hf2p\hf2p_definitions.h>
#include <game\multiplayer_definitions.h>

c_backend_data_cache g_backend_data_cache;

s_cached_public_data::s_cached_public_data()
    : loadouts()
    , customisation()
{

}

void s_cached_public_data::write_configuration(s_s3d_player_container* out_container, s_s3d_player_customization* out_customisation, s_player_appearance* out_appearance)
{
    ASSERT(out_container);
    ASSERT(out_customisation);
    ASSERT(out_appearance);

    for (long loadout_index = 0; loadout_index < k_maximum_loadouts; loadout_index++)
    {
        loadouts[loadout_index].write_loadout(&out_container->loadouts[loadout_index], &out_container->modifiers[loadout_index]);
    }

    customisation.write_customisation(out_customisation);

    e_gender gender = out_container->loadouts[out_customisation->active_loadout_index].gender.get();
    SET_BIT(out_appearance->flags, 0, gender == _gender_female);
    out_appearance->player_model_choice = 0; // $TODO:
    out_appearance->service_tag = L""; // $TODO:
}

void c_backend_data_cache::clear_title_instances()
{
    m_armor_items.clear();
    m_weapons.clear();
    m_grenades.clear();
    m_boosters.clear();
    m_consumables.clear();
    m_colours.clear();
}

void c_backend_data_cache::refresh_consumable_costs()
{
    m_consumable_costs.clear();

    long equipment_count = multiplayer_globals_get_equipment_count();
    if (equipment_count <= 0)
    {
        return;
    }
    m_consumable_costs.resize(equipment_count);

    // on map load, clear costs and pre allocate vector
    for (auto& cached_consumable : m_consumables)
    {
        s_cached_consumable& consumable = cached_consumable.second;
        m_consumable_costs[consumable.consumable_index] = consumable.costs;
    }
}

void c_backend_data_cache::refresh_scoring_events()
{
    s_multiplayer_runtime_globals_definition* runtime = scenario_multiplayer_globals_try_and_get_runtime_data();
    ulong maximum_wp_events = runtime->earn_wp_events.count();
    for (auto& scoring_event : g_backend_data_cache.m_scoring_events)
    {
        ulong event_index = scoring_event.event_index;

        if (VALID_INDEX(event_index, maximum_wp_events))
        {
            s_multiplayer_event_response_definition& wp_event = runtime->earn_wp_events[event_index];
            wp_event.earned_wp = static_cast<short>(scoring_event.xp_reward);
        }
    }
}

s_cached_public_data* const c_backend_data_cache::public_data_get(qword user_id)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    if (m_public_data.find(user_id) == m_public_data.end())
    {
        return NULL;
    }

    return &m_public_data[user_id];
}

void c_backend_data_cache::user_data_remove(qword user_id)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    // check if user exists in public data map
    if (m_public_data.find(user_id) == m_public_data.end())
    {
        return;
    }
    m_public_data.erase(user_id);
}

s_backend_loadout* const c_backend_data_cache::loadout_get(qword user_id, long loadout_index)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    if (m_public_data.find(user_id) == m_public_data.end())
    {
        return NULL;
    }

    return &m_public_data[user_id].loadouts[loadout_index];
}

void c_backend_data_cache::loadout_cache(qword user_id, s_backend_loadout* const loadout, long loadout_index)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    // assign to existing entry if it exists
    if (m_public_data.find(user_id) != m_public_data.end())
    {
        m_public_data[user_id].loadouts[loadout_index] = *loadout;
    }
    // create entry if one doesn't exist
    else
    {
        s_cached_public_data public_data;
        public_data.loadouts[loadout_index] = *loadout;
        m_public_data.insert_or_assign(user_id, public_data);
    }
}

s_backend_customisation* const c_backend_data_cache::customisation_get(qword user_id)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    // check if user exists in loadouts map
    if (m_public_data.find(user_id) == m_public_data.end())
    {
        return NULL;
    }

    return &m_public_data[user_id].customisation;
}

void c_backend_data_cache::customisation_cache(qword user_id, s_backend_customisation* const customisation)
{
    std::lock_guard<std::mutex> lock(m_public_data_mutex);

    // assign to existing entry if it exists
    if (m_public_data.find(user_id) != m_public_data.end())
    {
        m_public_data[user_id].customisation = *customisation;
    }
    // create entry if one doesn't exist
    else
    {
        s_cached_public_data public_data;
        public_data.customisation = *customisation;
        m_public_data.insert_or_assign(user_id, public_data);
    }
}

s_modifier::s_modifier(e_modifiers modifier, float value)
{
    this->modifier = modifier;
    this->value = value;
}
s_modifier::s_modifier(std::string modifier, float value)
{
    this->modifier = (e_modifiers)NONE;
    this->value = value;

    // convert string name to index
    for (long i = 0; i < k_modifiers_count; i++)
    {
        if (modifier == modifier_get_name((e_modifiers)i))
        {
            this->modifier = (e_modifiers)i;
            break;
        }
    }
}

s_cached_armor_item::s_cached_armor_item()
    : gender_armor()
    , race_id(NONE)
    , modifiers()
    , can_colour_regions()
{

}

s_cached_armor_item::s_cached_armor_item(s_title_instance& instance)
    : gender_armor()
    , race_id(NONE)
    , modifiers()
    , can_colour_regions()
{
    // name to index conversion via globals
    s_game_globals* game_globals = scenario_try_and_get_game_globals();
    if (!game_globals)
    {
        return;
    }

    hf2p_globals_definition* hf2p_globals = (hf2p_globals_definition*)tag_get(HF2P_GLOBALS_TAG, game_globals->armor_globals.index);
    if (!hf2p_globals)
    {
        return;
    }

    // s_properties_search_results
    std::vector<std::string> search_for_props =
    {
        "NAME",
        "RACE_ID",
        "ARMOR_MODIFIERS_VALUES",
        "ARMOR_MODIFIERS_LIST",
        "CAN_CUSTOMIZE_PRIMARY",
        "CAN_CUSTOMIZE_SECONDARY",
        "CAN_CUSTOMIZE_VISOR",
        "CAN_CUSTOMIZE_LIGHTS",
        "CAN_CUSTOMIZE_HOLOGRAM"
    };

    s_title_instance::s_properties::s_search_results properties = instance.get_properties(search_for_props);

    std::string armor_name = properties.get_string("NAME");
    race_id = properties.get_integer("RACE_ID");
    std::string modifiers_values = properties.get_string("ARMOR_MODIFIERS_VALUES");
    std::string modifiers_list = properties.get_string("ARMOR_MODIFIERS_LIST");
    can_colour_regions[_armor_color_primary] = properties.get_boolean("CAN_CUSTOMIZE_PRIMARY");
    can_colour_regions[_armor_color_secondary] = properties.get_boolean("CAN_CUSTOMIZE_SECONDARY");
    can_colour_regions[_armor_color_visor] = properties.get_boolean("CAN_CUSTOMIZE_VISOR");
    can_colour_regions[_armor_color_lights] = properties.get_boolean("CAN_CUSTOMIZE_LIGHTS");
    can_colour_regions[_armor_color_holo] = properties.get_boolean("CAN_CUSTOMIZE_HOLOGRAM");

    if (race_id == NONE || armor_name == "")
    {
        return;
    }
    if (!VALID_INDEX(race_id, hf2p_globals->race_armors.count()))
    {
        return;
    }

    auto& race_armors = hf2p_globals->race_armors[race_id];

    for (auto& gender : race_armors.genders)
    {
        long armor_index = NONE;
        for (long armor_index = 0; armor_index < gender.armor_objects.count(); armor_index++)
        {
            auto& armor_object = gender.armor_objects[armor_index];

            if (armor_name == armor_object.name.get_string())
            {
                gender_armor.push_back((e_armor)armor_index);
                break;
            }
        }
    }

    // split modifiers list & values strings into vectors
    std::vector<std::string> list;
    std::stringstream list_stream(modifiers_list.data());
    std::string list_item;
    while (std::getline(list_stream, list_item, ';'))
    {
        if (!list_item.empty()) // skip empty tokens (e.g. from trailing ';')
        {
            // remove mod_ from the start of the modifier name to later use with modifier_get_name
            const std::string prefix = "mod_";
            if (list_item.rfind(prefix, 0) == 0)
            {
                list_item = list_item.substr(prefix.size());
            }
            list.push_back(list_item);
        }
    }
    std::vector<float> values;
    std::stringstream values_stream(modifiers_values.data());
    std::string value_item;
    while (std::getline(values_stream, value_item, ';'))
    {
        if (!value_item.empty())
        {
            values.push_back(std::stof(value_item));
        }
    }

    if (list.size() != values.size())
    {
        return;
    }

    for (ulong modifier_index = 0; modifier_index < list.size(); modifier_index++)
    {
        modifiers.push_back({ list[modifier_index], values[modifier_index] });
    }
};
