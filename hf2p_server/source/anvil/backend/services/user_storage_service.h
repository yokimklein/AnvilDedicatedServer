#pragma once
#include <anvil\backend\backend.h>

enum e_user_storage_container
{
    _container_spartan0,
    _container_spartan1,
    _container_spartan2,
    _container_customizations,

    k_user_storage_container_count
};

class c_backend::user_storage_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    class get_public_data
    {
    public:
        inline static s_request_info m_status;

        struct s_user_id
        {
            qword Id;
        };

        struct s_request : s_backend_request
        {
            std::vector<s_user_id> users;
            std::string containerName;

            std::string to_json() override;
        };
        static void request(std::vector<qword> user_ids, e_user_storage_container container);

        struct s_response
        {
            struct s_per_user_data
            {
                long Layout;
                long Version;
                std::vector<char> Data;
            };

            struct s_public_data
            {
                s_user_id User;
                s_per_user_data PerUserData;
            };

            std::vector<s_public_data> body; // 'body' doesn't exist in response, its just a naked object
        };
    protected:
        static void response(s_backend_response* response);
    };

private:
    inline static resolved_endpoint* m_endpoint;
};
