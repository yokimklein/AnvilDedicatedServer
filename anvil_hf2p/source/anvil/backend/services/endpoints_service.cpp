#include "endpoints_service.h"
#include <anvil\backend\services\authorization_service.h>
#include <cseries\cseries_events.h>

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
    s_versions versions[3];
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

typedef c_backend::endpoints_service::get_authorization_endpoints_and_date::s_response s_response_get_endpoints;
s_response_get_endpoints tag_invoke(boost::json::value_to_tag<s_response_get_endpoints>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_response_get_endpoints response
    {
        boost::json::value_to<std::vector<s_endpoint_response>>(obj.at("Endpoints"))
    };
    return response;
};

void c_backend::endpoints_service::get_authorization_endpoints_and_date::response(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        m_status.status = _request_status_failed;
        return;
    }

    s_response body = boost::json::value_to<s_response>(response->data);
    long endpoints_count = body.Endpoints.size();

    if (endpoints_count == 0)
    {
        event(_event_warning, "backend:endpoints_service: received 0 endpoints!");
        m_status.status = _request_status_failed;
        return;
    }

    // Use first default endpoint, or use the first endpoint
    s_endpoint_response& authorisation_endpoint = body.Endpoints[0];
    for (auto& endpoint : body.Endpoints)
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
    m_status.status = _request_status_received;
    event(_event_message, "backend:endpoints_service: received %d endpoints, using [%s]", endpoints_count, authorisation_endpoint.Name.c_str());
}
