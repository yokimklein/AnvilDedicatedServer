#include "cache.h"
#include <anvil\backend\services\title_resource_service.h>
#include <scenario\scenario.h>
#include <game\game_globals.h>
#include <cache\cache_files.h>
#include <hf2p\hf2p_definitions.h>

s_backend_data_cache g_backend_data_cache;

void s_backend_data_cache::clear_title_instances()
{
    armor_items.clear();
}

s_armor_item::s_armor_item(s_title_instance& instance)
    : can_colour_regions()
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

    race_id = NONE;
    std::string_view armor_name = "";
    std::string_view modifiers_values = "";
    std::string_view modifiers_list = "";

    // Get relevant properties
    for (auto& prop : instance.props)
    {
        if (race_id == NONE && prop.name == "RACE_ID")
        {
            race_id = std::get<long>(prop.value);
            continue;
        }
        if (armor_name == "" && prop.name == "NAME")
        {
            armor_name = std::get<std::string>(prop.value);
            continue;
        }
        if (modifiers_values == "" && prop.name == "ARMOR_MODIFIERS_VALUES")
        {
            modifiers_values = std::get<std::string>(prop.value);
            continue;
        }
        if (modifiers_list == "" && prop.name == "ARMOR_MODIFIERS_LIST")
        {
            modifiers_list = std::get<std::string>(prop.value);
            continue;
        }
        if (prop.name == "CAN_CUSTOMIZE_PRIMARY")
        {
            can_colour_regions[_armor_color_primary] = std::get<long>(prop.value) == 1 ? true : false;
            continue;
        }
        if (prop.name == "CAN_CUSTOMIZE_SECONDARY")
        {
            can_colour_regions[_armor_color_secondary] = std::get<long>(prop.value) == 1 ? true : false;
            continue;
        }
        if (prop.name == "CAN_CUSTOMIZE_VISOR")
        {
            can_colour_regions[_armor_color_visor] = std::get<long>(prop.value) == 1 ? true : false;
            continue;
        }
        if (prop.name == "CAN_CUSTOMIZE_LIGHTS")
        {
            can_colour_regions[_armor_color_lights] = std::get<long>(prop.value) == 1 ? true : false;
            continue;
        }
        if (prop.name == "CAN_CUSTOMIZE_HOLOGRAM")
        {
            can_colour_regions[_armor_color_holo] = std::get<long>(prop.value) == 1 ? true : false;
            continue;
        }
    }
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
        long modifier = NONE;
        for (long i = 0; i < k_modifiers_count; i++)
        {
            if (list[modifier_index] == modifier_get_name((e_modifiers)i))
            {
                modifier = i;
                break;
            }
        }
        if (modifier == NONE)
        {
            continue;
        }
        modifiers.push_back({ (e_modifiers)modifier, values[modifier_index] });
    }
};