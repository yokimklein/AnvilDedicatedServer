#pragma once
#include <anvil\backend\backend.h>

class c_backend::private_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    class register_game_server
    {
    public:
        struct s_request : s_backend_request
        {
            std::string secureAddr;

            std::string to_json() override;
        };
        static void request(std::string secureAddr);

    protected:
        static void response(s_backend_response* response);
    };

    class unregister_game_server
    {
    public:
        struct s_request : s_backend_request
        {
            std::string secureAddr;

            std::string to_json() override;
        };
        static void request(std::string secureAddr);

    protected:
        static void response(s_backend_response* response);
    };

    class update_game_server
    {
    public:
        struct s_request : s_backend_request
        {
            std::string secureAddr;
            std::string serverAddr;
            long serverPort;
            std::string playlistId;

            std::string to_json() override;
        };
        static void request(std::string secureAddr, std::string serverAddr, long serverPort, std::string playlistId);

    protected:
        static void response(s_backend_response* response);
    };

    class retrieve_lobby_members
    {
    public:
        struct s_request : s_backend_request
        {
            std::string lobbyId;

            std::string to_json() override;
        };
        static void request(std::string lobbyId);

    protected:
        static void response(s_backend_response* response);
    };

private:
    inline static resolved_endpoint* m_endpoint;
};