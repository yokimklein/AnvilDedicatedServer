#include "title_resource_service.h"
#include <anvil\backend\cache.h>
#include <networking\network_time.h>

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

typedef c_backend::title_resource_service::s_title_instance s_title_instance;
typedef c_backend::title_resource_service::s_title_instance::s_properties s_properties;
s_properties tag_invoke(boost::json::value_to_tag<s_properties>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_properties prop;
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
        boost::json::value_to<std::vector<s_properties>>(obj.at("props"))
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

        // $TODO: Process TIs here into data cache

        g_backend_data_cache.last_update = network_time_get();
        g_backend_data_cache.valid = true;
        m_status.status = _request_status_received;
        return;
    }

    m_status.status = _request_status_failed;
    return;
}
