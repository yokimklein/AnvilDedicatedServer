#pragma once
#include <anvil\backend\backend.h>
#include <anvil\backend\request_info.h>

class c_backend::title_resource_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    class get_title_configuration
    {
    public:
        inline static s_request_info m_status;

        struct s_request : s_backend_request
        {
            std::string combinationHash; // $TODO: ignored by API

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            struct s_title_instance
            {
                std::string name;
                std::string className;
                std::vector<long> parents;
            };

            std::vector<s_title_instance> title_instances; // title_instances name not in response
        };
    protected:
        static void response(s_backend_response* response);
    };

private:
    inline static resolved_endpoint* m_endpoint;
};
