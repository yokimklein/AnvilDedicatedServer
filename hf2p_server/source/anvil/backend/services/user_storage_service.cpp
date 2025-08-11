#include "user_storage_service.h"
#include <hf2p\loadouts.h>
#include <anvil\backend\backend.h>
#include <iostream>
#include <anvil\backend\cache.h>

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

void c_backend::user_storage_service::get_public_data::request(qword* user_ids, long user_count, e_user_storage_container container)
{
    ASSERT(VALID_INDEX(container, k_user_storage_container_count));
    if (!m_endpoint->m_resolved || !VALID_INDEX(container, k_user_storage_container_count))
    {
        return;
    }

    s_request request_body;
    request_body.containerName = k_user_storage_container_names[container];
    for (long user_index = 0; user_index < user_count; user_index++)
    {
        request_body.users.push_back({ user_ids[user_index]});
    }

    ulong request_identifier = c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/d7e31cf042024b22ac09d2a42c0403cb.svc/dbcbbedf8eb748b19766e1f7e25e6e46",
        &response,
        *m_endpoint,
        true
    );

    if (request_identifier != NONE)
    {
        std::lock_guard<std::mutex> lock(m_request_queue_mutex);
        m_request_queue.insert({ request_identifier, container });
    }
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

void c_backend::user_storage_service::get_public_data::response(s_backend_response* response)
{
    if (response->retCode == _backend_success && response->request_identifier != NONE)
    {
        s_response body = boost::json::value_to<s_response>(response->data);

        // retrieve container type from queue and remove entry
        std::lock_guard<std::mutex> lock(m_request_queue_mutex);
        e_user_storage_container container_type = m_request_queue[response->request_identifier];
        m_request_queue.erase(response->request_identifier);

        switch (container_type)
        {
            case _container_spartan0:
            case _container_spartan1:
            case _container_spartan2:
            {
                for (auto& user : body.body)
                {
                    // If the size is different to what we're expecting, ignore it
                    ASSERT(user.PerUserData.Data.size() == sizeof(s_backend_loadout));
                    if (user.PerUserData.Data.size() != sizeof(s_backend_loadout))
                    {
                        continue;
                    }
                    s_backend_loadout loadout;
                    memcpy(&loadout, user.PerUserData.Data.data(), user.PerUserData.Data.size());
                    loadout.null_terminate_buffers(); // make sure all strings are null terminated in case of corruption

                    // $NOTE: this works so long as the spartan containers in e_user_storage_container match with the loadout indices
                    g_backend_data_cache.loadout_cache(user.User.Id, &loadout, container_type);
                }
                break;
            }
            case _container_customizations:
            {
                for (auto& user : body.body)
                {
                    // If the size is different to what we're expecting, ignore it
                    ASSERT(user.PerUserData.Data.size() == sizeof(s_backend_customisation));
                    if (user.PerUserData.Data.size() != sizeof(s_backend_customisation))
                    {
                        continue;
                    }
                    s_backend_customisation customisation;
                    memcpy(&customisation, user.PerUserData.Data.data(), user.PerUserData.Data.size());
                    customisation.null_terminate_buffers(); // make sure all strings are null terminated in case of corruption
                    g_backend_data_cache.customisation_cache(user.User.Id, &customisation);
                }
                break;
            }
            default:
                break;
        }

        // version number increment signifies a change, except the API currently doesn't increment it

         m_status.status = _request_status_received;
         return;
    }

    m_status.status = _request_status_failed;
    return;
}
