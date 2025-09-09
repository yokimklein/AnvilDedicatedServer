#include "authorization_service.h"
#include <anvil\config.h>
#include <anvil\backend\lobby.h>
#include <networking\network_time.h>
#include <cseries\cseries_events.h>

void c_backend::authorization_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::authorization_service::endpoint()
{
    return *m_endpoint;
}

typedef c_backend::authorization_service::enqueue::s_request::s_signature s_signature;
void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_signature const& signature)
{
    boost::json::object out;
    out["Id"] = signature.Id;
    json_value = std::move(out);
}

typedef c_backend::authorization_service::enqueue::s_response::s_token s_token;
s_token tag_invoke(boost::json::value_to_tag<s_token>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_token token
    {
        obj.at("Token").as_string().c_str(),
        static_cast<int>(obj.at("State").as_int64())
    };
    return token;
}

std::string c_backend::authorization_service::enqueue::s_request::to_json()
{
    boost::json::object out;
    out["login"] = login;
    out["password"] = password;
    out["provider"] = provider;
    out["signature"] = boost::json::value_from(signature);
    out["versions"] = boost::json::value_from(versions);
    return boost::json::serialize(out);
}

void c_backend::authorization_service::enqueue::request()
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    authorization_service::m_session_state = _backend_session_authenticating;

    s_transport_secure_address signature;
    anvil_get_server_identifier(&signature);

    s_request request_body;
    request_body.login = g_anvil_configuration["account"];
    request_body.password = g_anvil_configuration["sign-in-code"];
    request_body.provider = "";
    request_body.signature.Id = transport_secure_address_get_string(&signature);
    s_versions versions[3];
    // $TODO: the API currently doesn't read the versions
    versions[0].ServiceName = "d7e31cf042024b22ac09d2a42c0403cb"; // UserStorageService
    versions[0].Version = 2;
    versions[0].MinorVersion = 0;
    for (long i = 0; i < NUMBEROF(versions); i++)
    {
        request_body.versions.push_back(versions[i]);
    }

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/AuthorizationService.svc/Enqueue",
        &response,
        *m_endpoint,
        false
    );
}

typedef c_backend::authorization_service::enqueue::s_response s_response_enqueue;
s_response_enqueue tag_invoke(boost::json::value_to_tag<s_response_enqueue>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_response_enqueue response
    {
        static_cast<long>(obj.at("Position").as_int64()),
        boost::json::value_to<s_token>(obj.at("Token"))
    };
    return response;
};

void c_backend::authorization_service::enqueue::response(s_backend_response* response)
{
    if (response->retCode == _backend_success)
    {
        s_response body = boost::json::value_to<s_response>(response->data);

        if (body.Position != 0 || body.Token.State != 2)
        {
            event(_event_critical, "backend:authorization_service: API attempted to hold game server in the login queue - this should never happen!");
        }
        else
        {
            m_authorisation_token = body.Token.Token;
            m_last_token_refresh = network_time_get();
            event(_event_status, "backend:authorization_service: enqueue successful");
            m_session_state = _backend_session_authenticating;
            m_status.status = _request_status_received;
            return;
        }
    }

    // failed
    m_session_state = _backend_session_offline;
    m_status.status = _request_status_failed;
    return;
}

std::string c_backend::authorization_service::dequeue::s_request::to_json()
{
    boost::json::object out;
    out["token"] = token;
    return boost::json::serialize(out);
}

void c_backend::authorization_service::dequeue::request()
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    authorization_service::m_session_state = _backend_session_establishing;

    s_request request_body;
    request_body.token = m_authorisation_token;

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/AuthorizationService.svc/Dequeue",
        &response,
        *m_endpoint,
        false
    );
}

typedef c_backend::authorization_service::dequeue::s_response s_response_dequeue;
s_response_dequeue tag_invoke(boost::json::value_to_tag<s_response_dequeue>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_response_dequeue response
    {
        obj.at("AuthorizationToken").as_string().c_str(),
        obj.at("DiagnosticToken").as_string().c_str(),
        boost::json::value_to<std::vector<s_endpoint_response>>(obj.at("TitleServers")),
        boost::json::value_to<std::vector<s_endpoint_response>>(obj.at("DiagnosticServices")),
        //obj.at("ContextToken").as_string().c_str() // $TODO:
    };
    return response;
}

// $TODO: use tag invoke to deserialize json strings to response structs in other responses
void c_backend::authorization_service::dequeue::response(s_backend_response* response)
{
    if (response->retCode == _backend_success)
    {
        s_response body = boost::json::value_to<s_response>(response->data);
        m_authorisation_token = body.AuthorizationToken;
        m_diagnostics_token = body.DiagnosticToken;
        m_last_token_refresh = network_time_get();
        //m_context_token = body.ContextToken;

        // Use first default endpoint, or use the first endpoint
        s_endpoint_response& title_server_endpoint = body.TitleServers[0];
        for (auto& endpoint : body.TitleServers)
        {
            if (endpoint.IsDefault)
            {
                title_server_endpoint = endpoint;
                break;
            }
        }

        g_backend_services->resolve(_endpoint_title_server, title_server_endpoint.IP, std::format("{}", title_server_endpoint.Port));

        m_session_state = _backend_session_online;
        m_status.status = _request_status_received;
        return;
    }

    m_session_state = _backend_session_offline;
    m_status.status = _request_status_failed;
    return;
}

std::string c_backend::authorization_service::refresh_tokens::s_request::to_json()
{
    boost::json::object out;
    out["tokens"] = boost::json::value_from(tokens);
    return boost::json::serialize(out);
}

void c_backend::authorization_service::refresh_tokens::request()
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    s_request request_body;
    request_body.tokens[0] = m_authorisation_token;
    request_body.tokens[1] = m_diagnostics_token;

    c_backend::make_request
    (
        request_body,
        http::verb::post,
        "/AuthorizationService.svc/RefreshTokens",
        &response,
        *m_endpoint,
        true
    );
}

typedef c_backend::authorization_service::refresh_tokens::s_response::s_token_refresh s_token_refresh;
s_token_refresh tag_invoke(boost::json::value_to_tag<s_token_refresh>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_token_refresh refresh
    {
        obj.at("OldValue").as_string().c_str(),
        obj.at("NewValue").as_string().c_str(),
    };
    return refresh;
}

typedef c_backend::authorization_service::refresh_tokens::s_response s_response_refresh_tokens;
s_response_refresh_tokens tag_invoke(boost::json::value_to_tag<s_response_refresh_tokens>, boost::json::value const& jv)
{
    s_response_refresh_tokens response
    {
        boost::json::value_to<std::vector<s_token_refresh>>(jv)
    };
    return response;
}

void c_backend::authorization_service::refresh_tokens::response(s_backend_response* response)
{
    if (response->retCode == _backend_success)
    {
        s_response body = boost::json::value_to<s_response>(response->data);

        if (body.tokens.size() != 2)
        {
            event(_event_error, "backend:authorization_service: received invalid number of tokens! [%d]", body.tokens.size());
        }
        else
        {
            m_authorisation_token = body.tokens[0].NewValue;
            m_diagnostics_token = body.tokens[1].NewValue;
            m_last_token_refresh = network_time_get();
            m_session_state = _backend_session_online;
            m_status.status = _request_status_received;
            return;
        }
    }

    m_session_state = _backend_session_offline;
    m_status.status = _request_status_failed;
    return;
}
