#include "endpoints_service.h"
#include <anvil\backend\services\authorization_service.h>

void c_backend::endpoints_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::endpoints_service::endpoint()
{
    return *m_endpoint;
}

typedef c_backend::endpoints_service::get_authorization_endpoints_and_date::s_request::s_versions s_versions;
void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_versions const& version)
{
    boost::json::object out;
    out["ServiceName"] = version.ServiceName;
    out["Version"] = version.Version;
    out["MinorVersion"] = version.MinorVersion;
    json_value = std::move(out);
}

typedef c_backend::endpoints_service::get_authorization_endpoints_and_date::s_response::s_authorization_endpoint s_authorization_endpoint;
c_backend::endpoints_service::get_authorization_endpoints_and_date::s_response::s_authorization_endpoint tag_invoke
(
    boost::json::value_to_tag<s_authorization_endpoint>,
    boost::json::value const& jv
)
{
    auto const& obj = jv.as_object();
    s_authorization_endpoint endpoint
    {
        obj.at("Name").as_string().c_str(),
        obj.at("IP").as_string().c_str(),
        static_cast<int>(obj.at("Port").as_int64()),
        static_cast<int>(obj.at("Protocol").as_int64()),
        obj.at("IsDefault").as_bool()
    };
    return endpoint;
}

std::string c_backend::endpoints_service::get_authorization_endpoints_and_date::s_request::to_json()
{
    boost::json::object out;
    out["provider"] = provider;
    out["versions"] = boost::json::value_from(versions);
    return boost::json::serialize(out);
}

void c_backend::endpoints_service::get_authorization_endpoints_and_date::request()
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    s_request request_body;
    request_body.provider = "";
    s_request::s_versions versions[3];
    versions[0].ServiceName = "AuthorizationService";
    versions[0].Version = 4;
    versions[0].MinorVersion = 14;
    versions[1].ServiceName = "d7e31cf042024b22ac09d2a42c0403cb"; // UserStorageService
    versions[1].Version = 2;
    versions[1].MinorVersion = 0;
    versions[2].ServiceName = "PrivateService";
    versions[2].Version = 1;
    versions[2].MinorVersion = 0;
    for (long i = 0; i < NUMBEROF(versions); i++)
    {
        request_body.versions.push_back(versions[i]);
    }

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/EndpointsDispatcherService.svc/GetAuthorizationEndpointsAndDate",
        &response,
        *m_endpoint,
        false
    );
}

void c_backend::endpoints_service::get_authorization_endpoints_and_date::response(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    if (!response->data.contains("Endpoints"))
    {
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": response returned no endpoints array!\n");
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    auto endpoints = boost::json::value_to<std::vector<s_authorization_endpoint>>(response->data.at("Endpoints"));
    long endpoints_count = endpoints.size();

    if (endpoints_count == 0)
    {
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received 0 endpoints!\n");
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    // Use first default endpoint, or use the first endpoint
    s_authorization_endpoint& authorisation_endpoint = endpoints[0];
    for (auto& endpoint : endpoints)
    {
        if (endpoint.IsDefault)
        {
            authorisation_endpoint = endpoint;
            break;
        }
    }
    // resolve auth service now we have the IP & port
    c_backend::authorization_service::endpoint().resolve(_endpoint_authorization, authorisation_endpoint.IP, std::format("{}", authorisation_endpoint.Port));
    m_authorization_endpoint_valid = true;
    m_eds_request_info.status = _request_status_received;

    printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received %d endpoints, using [%s]\n", endpoints_count, authorisation_endpoint.Name.c_str());

    // don't currently need this
    //if (response->data.contains("DateTime"))
    //{
    //    qword date_time = response->data.at("DateTime").as_uint64();
    //}
}
