#include "user_storage_service.h"
#include <hf2p\loadouts.h>
#include <anvil\backend\backend.h>

const char* k_user_storage_container_names[k_user_storage_container_count] =
{
    "sc0",
    "sc1",
    "sc2",
    "customizations"
};

void c_backend::user_storage_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::user_storage_service::endpoint()
{
    return *m_endpoint;
}

typedef c_backend::user_storage_service::get_public_data::s_user_id s_user_id;
void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_user_id const& user_id)
{
    boost::json::object out;
    out["Id"] = user_id.Id;
    json_value = std::move(out);
}

std::string c_backend::user_storage_service::get_public_data::s_request::to_json()
{
    boost::json::object out;
    out["users"] = boost::json::value_from(users);
    out["containerName"] = containerName;
    return boost::json::serialize(out);
}

void c_backend::user_storage_service::get_public_data::request(std::vector<qword> user_ids, e_user_storage_container container)
{
    ASSERT(VALID_INDEX(container, k_user_storage_container_count));
    if (!m_endpoint->m_resolved || !VALID_INDEX(container, k_user_storage_container_count))
    {
        return;
    }

    s_request request_body;
    request_body.containerName = k_user_storage_container_names[container];
    for (qword& user_id : user_ids)
    {
        request_body.users.push_back({ user_id });
    }

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/d7e31cf042024b22ac09d2a42c0403cb.svc/dbcbbedf8eb748b19766e1f7e25e6e46",
        &response,
        *m_endpoint,
        true
    );
}

s_user_id tag_invoke(boost::json::value_to_tag<s_user_id>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_user_id refresh
    {
        // Fails to parse when using as_uint64?
        static_cast<qword>(obj.at("Id").as_int64())
    };
    return refresh;
}

typedef c_backend::user_storage_service::get_public_data::s_response::s_per_user_data s_per_user_data;
s_per_user_data tag_invoke(boost::json::value_to_tag<s_per_user_data>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_per_user_data refresh
    {
        static_cast<long>(obj.at("Layout").as_int64()),
        static_cast<long>(obj.at("Version").as_int64()),
        boost::json::value_to<std::vector<char>>(obj.at("Data"))
    };
    return refresh;
}

typedef c_backend::user_storage_service::get_public_data::s_response::s_public_data s_public_data;
s_public_data tag_invoke(boost::json::value_to_tag<s_public_data>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_public_data refresh
    {
        boost::json::value_to<s_user_id>(obj.at("User")),
        boost::json::value_to<s_per_user_data>(obj.at("PerUserData"))
    };
    return refresh;
}

typedef c_backend::user_storage_service::get_public_data::s_response s_response_get_public_data;
s_response_get_public_data tag_invoke(boost::json::value_to_tag<s_response_get_public_data>, boost::json::value const& jv)
{
    s_response_get_public_data response
    {
        boost::json::value_to<std::vector<s_public_data>>(jv)
    };
    return response;
};

// $TODO: need to remember which container was requested for response
// $TODO: need to be able to fire multiple requests at once, could put them in a vector?
// create GetPublicData request queue which stores container name type, status, this is what ticks update in backend.cpp
// 
// /TitleResourceService.svc/GetTitleConfiguration
// /ee3aec5524854d6e918307d24a14623d.svc/c3bf25f34b72499ca307c6e4431c79c1
// $TODO: grab TIs next to use to convert loadouts, only bother processing and caching armour, weapon, grenade, support, tactical, colours & scoring events
// ARMOR_ITEM, WEAPON, GRENADE, BOOSTER, CONSUMABLE, COLOR, SCORING_EVENT

void c_backend::user_storage_service::get_public_data::response(s_backend_response* response)
{
    if (response->retCode == _backend_success)
    {
        s_response body = boost::json::value_to<s_response>(response->data);

        // $TODO: convert data here
        s_api_user_loadout loadout;
        s_api_user_customisation customisation;

         m_status.status = _request_status_received;
         return;
    }

    m_status.status = _request_status_failed;
    return;
}
