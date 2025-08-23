#include "title_resource_service.h"
#include <anvil\backend\cache.h>
#include <networking\network_time.h>
#include <scenario\scenario.h>
#include <game\game_globals.h>
#include <game\multiplayer_definitions.h>

s_title_instance::s_properties::s_search_results s_title_instance::get_properties(std::vector<std::string> prop_names)
{
    // $TODO: order found_props by prop_names order?
    std::vector<std::string> search_list = prop_names;
    s_title_instance::s_properties::s_search_results found_props;

    for (auto& prop : props)
    {
        bool found = false;
        long found_index = NONE;
        for (ulong prop_index = 0; prop_index < search_list.size(); prop_index++)
        {
            auto& prop_name = search_list[prop_index];
            if (prop.name == prop_name)
            {
                found_props.results.push_back(prop);
                found = true;
                found_index = prop_index;
                break;
            }
        }

        // Remove found index from search list, no point in searching for it again
        if (found)
        {
            search_list.erase(search_list.begin() + found_index);
            found = false;
            found_index = NONE;
        }
    }

    return found_props;
}

s_title_instance::s_properties* const s_title_instance::s_properties::s_search_results::get_property(const char* name)
{
    for (ulong prop_index = 0; prop_index < results.size(); prop_index++)
    {
        if (results[prop_index].name == name)
        {
            return &results[prop_index];
        }
    }
    return NULL;
}

long s_title_instance::s_properties::s_search_results::get_integer(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return NONE;
    }
    ASSERT(prop->type == _title_property_integer);
    return std::get<long>(prop->value);
}

bool s_title_instance::s_properties::s_search_results::get_boolean(const char* name)
{
    return get_integer(name) == 1 ? true : false;
}

float s_title_instance::s_properties::s_search_results::get_float(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return 0.0f;
    }
    ASSERT(prop->type == _title_property_float);
    return std::get<float>(prop->value);
}

std::string s_title_instance::s_properties::s_search_results::get_string(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return "";
    }
    ASSERT(prop->type == _title_property_string);
    return std::get<std::string>(prop->value);
}

long64 s_title_instance::s_properties::s_search_results::get_long(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return 0LL;
    }
    ASSERT(prop->type == _title_property_long);
    return std::get<long64>(prop->value);
}

std::vector<long> s_title_instance::s_properties::s_search_results::get_integer_list(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return {};
    }
    ASSERT(prop->type == _title_property_integer_list);
    return std::get<std::vector<long>>(prop->value);
}

std::vector<std::string> s_title_instance::s_properties::s_search_results::get_string_list(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return {};
    }
    ASSERT(prop->type == _title_property_string_list);
    return std::get<std::vector<std::string>>(prop->value);
}

std::vector<s_title_instance> s_title_instance::s_properties::s_search_results::get_object_list(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return {};
    }
    ASSERT(prop->type == _title_property_object_list);
    return std::get<std::vector<s_title_instance>>(prop->value);
}

std::vector<float> s_title_instance::s_properties::s_search_results::get_float_list(const char* name)
{
    s_title_instance::s_properties* const prop = get_property(name);
    if (!prop)
    {
        return {};
    }
    ASSERT(prop->type == _title_property_float_list);
    return std::get<std::vector<float>>(prop->value);
}

void c_backend::title_resource_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::title_resource_service::endpoint()
{
    return *m_endpoint;
}

std::string c_backend::title_resource_service::get_title_configuration::s_request::to_json()
{
    boost::json::object out;
    out["combinationHash"] = combinationHash;
    return boost::json::serialize(out);
}

s_title_instance::s_properties tag_invoke(boost::json::value_to_tag<s_title_instance::s_properties>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_title_instance::s_properties prop;
    prop.name = obj.at("name").as_string().c_str();
    prop.type = static_cast<e_title_property_types>(obj.at("type").as_int64());

    switch (prop.type)
    {
        case _title_property_integer:
            prop.value = static_cast<long>(obj.at("intVal").as_int64());
            break;
        case _title_property_float:
            prop.value = static_cast<float>(obj.at("floatVal").as_double());
            break;
        case _title_property_string:
            prop.value = obj.at("strVal").as_string().c_str();
            break;
        case _title_property_long:
            prop.value = obj.at("longVal").as_int64();
            break;
        case _title_property_integer_list:
            prop.value = boost::json::value_to<std::vector<long>>(obj.at("intList"));
            break;
        case _title_property_string_list:
            prop.value = boost::json::value_to<std::vector<std::string>>(obj.at("strList"));
            break;
        case _title_property_object_list:
            prop.value = boost::json::value_to<std::vector<s_title_instance>>(obj.at("objList"));
            break;
        case _title_property_float_list:
            prop.value = boost::json::value_to<std::vector<float>>(obj.at("floatList"));
            break;
        default:
            break;
    }

    return prop;
};

s_title_instance tag_invoke(boost::json::value_to_tag<s_title_instance>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_title_instance title_instance
    {
        // name can be null in objLists
        !obj.at("name").is_null() ? obj.at("name").as_string().c_str() : "",
        obj.at("className").as_string().c_str(),
        boost::json::value_to<std::vector<std::string>>(obj.at("parents")),
        boost::json::value_to<std::vector<s_title_instance::s_properties>>(obj.at("props"))
    };
    return title_instance;
};

typedef c_backend::title_resource_service::get_title_configuration::s_response s_response_get_title_configuration;
s_response_get_title_configuration tag_invoke(boost::json::value_to_tag<s_response_get_title_configuration>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_response_get_title_configuration response
    {
        obj.at("combinationHash").as_string().c_str(),
        boost::json::value_to<std::vector<s_title_instance>>(obj.at("instances"))
    };
    return response;
};

void c_backend::title_resource_service::get_title_configuration::request()
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    s_request request_body;
    request_body.combinationHash = ""; // $TODO: can we filter TIs by this?

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/ee3aec5524854d6e918307d24a14623d.svc/c3bf25f34b72499ca307c6e4431c79c1",
        &response,
        *m_endpoint,
        true
    );
}

void c_backend::title_resource_service::get_title_configuration::response(s_backend_response* response)
{
    if (response->retCode == _backend_success)
    {
        s_response body = boost::json::value_to<s_response>(response->data);

        c_backend_data_cache::clear_title_instances();

        // Pull maximum indices from tags to check if TI IDs are valid
        ulong maximum_weapons = 0;
        ulong maximum_grenades = 0;
        ulong maximum_consumables = 0;
        ulong maximum_wp_events = 0;
        s_game_globals* game_globals = scenario_try_and_get_game_globals();
        s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
        s_multiplayer_runtime_globals_definition* runtime = scenario_multiplayer_globals_try_and_get_runtime_data();
        if (universal)
        {
            maximum_weapons = universal->weapon_selections.count();
            maximum_consumables = universal->equipment.count();
        }
        if (game_globals)
        {
            maximum_grenades = game_globals->grenades.count();
        }
        if (runtime)
        {
            maximum_wp_events = runtime->earn_wp_events.count();
        }

        // Process TIs into data cache
        for (s_title_instance& instance : body.instances)
        {
            // Find instance type
            long instance_type = _instance_invalid;
            for (instance_type = _instance_invalid; instance_type < k_title_instance_game_class_count; instance_type++)
            {
                if (instance.className == K_TITLE_INSTANCE_GAME_CLASS[instance_type])
                {
                    break;
                }
            }

            // process instances
            switch (instance_type)
            {
                case _instance_armor_item:
                {
                    s_cached_armor_item armor(instance);
                    g_backend_data_cache.m_armor_items.insert({ instance.name, armor });
                    break;
                }
                case _instance_weapon:
                {
                    ulong weapon = instance.get_properties({ "ID" }).get_integer("ID");
                    if (VALID_INDEX(weapon, maximum_weapons))
                    {
                        g_backend_data_cache.m_weapons.insert({ instance.name, (e_weapon)weapon });
                    }
                    break;
                }
                case _instance_grenade:
                {
                    ulong grenade = instance.get_properties({ "ID" }).get_integer("ID");
                    if (VALID_INDEX(grenade, maximum_grenades))
                    {
                        g_backend_data_cache.m_grenades.insert({ instance.name, (e_grenade)grenade });
                    }
                    break;
                }
                case _instance_booster:
                {
                    std::vector<s_modifier> modifiers;
                    auto results = instance.get_properties({ "NAME", "MODIFIERS" });
                    auto modifiers_list = results.get_object_list("MODIFIERS");

                    for (auto& modifier : modifiers_list)
                    {
                        auto value_results = modifier.get_properties({ "NAME", "VALUE" });
                        modifiers.push_back({ value_results.get_string("NAME"), value_results.get_float("VALUE") });
                    }

                    g_backend_data_cache.m_boosters.insert({ instance.name, modifiers });
                    break;
                }
                case _instance_consumable:
                {
                    auto results = instance.get_properties({ "CONSUMABLE_INDEX", "ENERGY_COST", "COOLDOWN", "COOLDOWN_INIT" });
                    ulong consumable_index = results.get_integer("CONSUMABLE_INDEX");
                    if (VALID_INDEX(consumable_index, maximum_consumables))
                    {
                        s_cached_consumable consumable;
                        consumable.consumable_index = (e_consumables)consumable_index;
                        consumable.costs.energy_cost = results.get_integer("ENERGY_COST");
                        consumable.costs.cooldown = static_cast<float>(results.get_integer("COOLDOWN"));
                        consumable.costs.cooldown_init = static_cast<float>(results.get_integer("COOLDOWN_INIT"));
                        g_backend_data_cache.m_consumables.insert({ instance.name, consumable });
                    }
                    break;
                }
                case _instance_color:
                {
                    // convert RGB bytes to 32-bit ARGB
                    auto results = instance.get_properties({ "COLOR_R", "COLOR_G", "COLOR_B" });
                    byte red = static_cast<byte>(results.get_integer("COLOR_R"));
                    byte green = static_cast<byte>(results.get_integer("COLOR_G"));
                    byte blue = static_cast<byte>(results.get_integer("COLOR_B"));

                    ulong colour =
                        (static_cast<ulong>(255) << 24) |
                        (static_cast<ulong>(red) << 16) |
                        (static_cast<ulong>(green) << 8) |
                        (static_cast<ulong>(blue) << 0);

                    g_backend_data_cache.m_colours.insert({ instance.name, colour });
                    break;
                }
                case _instance_scoring_event:
                {
                    auto results = instance.get_properties({ "SCORING_EVENT_ID", "SCORING_REWARD_WP" });
                    ulong event_index = results.get_integer("SCORING_EVENT_ID");
                    short event_reward = static_cast<short>(results.get_integer("SCORING_REWARD_WP"));

                    if (VALID_INDEX(event_index, maximum_wp_events))
                    {
                        // cache data to use on map reload
                        g_backend_data_cache.m_scoring_events.push_back({ event_index, event_reward });

                        // set data immediately
                        s_multiplayer_event_response_definition& wp_event = runtime->earn_wp_events[event_index];
                        wp_event.earned_wp = static_cast<short>(event_reward);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        g_backend_data_cache.m_last_update = network_time_get();
        g_backend_data_cache.m_valid = true;
        m_status.status = _request_status_received;
        return;
    }

    m_status.status = _request_status_failed;
    return;
}
