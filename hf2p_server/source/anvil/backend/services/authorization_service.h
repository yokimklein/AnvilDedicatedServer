#pragma once
#include <anvil\backend\backend.h>

enum e_backend_session_state
{
    _backend_session_offline,
    _backend_session_authenticating, // enqueue sent
    _backend_session_establishing, // dequeue sent
    _backend_session_online, // finished

    k_backend_session_state_count
};

class c_backend::authorization_service
{
public:
	static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    inline static std::atomic<e_backend_session_state> m_session_state = _backend_session_offline;

    class enqueue
    {
    public:
        inline static s_request_info m_status;

        struct s_request : s_backend_request
        {
            struct s_signature
            {
                std::string Id;
            };

            std::string login;
            std::string password;
            std::string provider;
            s_signature signature;
            std::vector<s_versions> versions;

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            struct s_token
            {
                std::string Token;
                long State;
            };
            
            long Position;
            s_token Token;
        };
    protected:
        static void response(s_backend_response* response);
    };

    class dequeue
    {
    public:
        inline static s_request_info m_status;

        struct s_request : s_backend_request
        {
            std::string token;

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            std::string AuthorizationToken;
            std::string DiagnosticToken;
            std::vector<s_endpoint_response> TitleServers;
            std::vector<s_endpoint_response> DiagnosticServices;
            //std::string ContextToken;
        };
    protected:
        static void response(s_backend_response* response);
    };

    class refresh_tokens
    {
    public:
        inline static s_request_info m_status;

        struct s_request : s_backend_request
        {
            std::string tokens[2];

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            struct s_token_refresh
            {
                std::string OldValue;
                std::string NewValue;
            };

            std::vector<s_token_refresh> tokens; // 'tokens' doesn't exist in response, its just a naked object
        };
    protected:
        static void response(s_backend_response* response);
    };

	// $TODO: encapsulate these
	inline static ulong m_last_token_refresh = NONE;
	inline static std::string m_authorisation_token;
	inline static std::string m_diagnostics_token;
	inline static std::string m_context_token;
	
private:
	inline static resolved_endpoint* m_endpoint;
};
