#pragma once
#include <anvil\backend\backend.h>
#include <anvil\backend\request_info.h>
#include <variant>

enum e_title_property_types
{
    _title_property_none,
    _title_property_integer,
    _title_property_float,
    _title_property_string,
    _title_property_long,
    _title_property_integer_list,
    _title_property_string_list,
    _title_property_object_list,
    _title_property_float_list,
    
    k_title_property_types_count
};

class c_backend::title_resource_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    struct s_title_instance
    {
        struct s_properties
        {
            std::string name;
            e_title_property_types type;
            std::variant
            <
                /*   intVal*/ long,
                /* floatVal*/ float,
                /*   strVal*/ std::string,
                /*  longVal*/ long64,
                /*  intList*/ std::vector<long>,
                /*  strList*/ std::vector<std::string>,
                /*  objList*/ std::vector<s_title_instance>,
                /*floatList*/ std::vector<float>
            >value;
        };

        std::string name;
        std::string className;
        std::vector<std::string> parents;
        std::vector<s_properties> props;
    };

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
            std::string combinationHash;
            std::vector<s_title_instance> instances; // title_instances name not in response
        };
    protected:
        static void response(s_backend_response* response);
    };

private:
    inline static resolved_endpoint* m_endpoint;
};
