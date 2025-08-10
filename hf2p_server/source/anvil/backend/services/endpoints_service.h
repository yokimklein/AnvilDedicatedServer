#pragma once
#include <anvil\backend\backend.h>

class c_backend::endpoints_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    class get_authorization_endpoints_and_date
    {
    public:
        struct s_request : s_backend_request
        {
            // $TODO: Anvil's API doesn't actually read the request body
            struct s_versions
            {
                std::string ServiceName;
                int Version;
                int MinorVersion;
            };

            std::string provider;
            std::vector<s_versions> versions;

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            struct s_authorization_endpoint
            {
                std::string Name;
                std::string IP;
                long Port;
                long Protocol;
                bool IsDefault;
            };
            std::vector<s_authorization_endpoint> Endpoints;
            qword DateTime;
        };
    protected:
        static void response(s_backend_response* response);
    };

    // $TODO: encapsulate these
    inline static s_request_info m_eds_request_info;
    inline static bool m_authorization_endpoint_valid;

private:
    inline static resolved_endpoint* m_endpoint;
};
